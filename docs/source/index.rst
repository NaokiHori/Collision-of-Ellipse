####################
Collision-of-Ellipse
####################

.. seealso::

   `Documentation for Japanese readers <https://qiita.com/NaokiHori/items/daf3fd191d51a7e682f8>`_.

**********
Motivation
**********

I was interested in the collisions between two ellipses when I developed `this library <https://github.com/NaokiHori/SimpleSolidSuspensionSolver>`_.

Namely, under this kind of circumstance,

.. image:: data/problem-setup.png
   :width: 400

I needed to consider

* whether they are overlapped or not,
* if they are, how much the penetration depth :math:`\delta` is.

I assume the ellipses are completely rigid (no deformation), and :math:`\delta` is sufficiently small compared to the characteristic sizes of the colliding ellipses.

.. note::

   The following is a compromised solution to obtain something *plausible* in my fluid-structure couplings.
   Namely, neither the detection nor the quantification of :math:`\delta` are rigorous and thus its application is limited.

****************************
Problem setup and difficulty
****************************

=============
Problem setup
=============

Equation describing an ellipse whose center is at the origin and the major / minor axes are :math:`a_0` and :math:`b_0` leads

.. math::

   \frac{x_e^2}{a_0^2}
   +
   \frac{y_e^2}{b_0^2}
   =
   1.

Here I introduce a parameter :math:`t_0` for simplicity to write the above equation differently:

.. math::

   \begin{pmatrix}
      x_e \\
      y_e
   \end{pmatrix}
   =
   \begin{pmatrix}
      a_0 \cos t_0 \\
      b_0 \sin t_0
   \end{pmatrix}.

In general, an ellipse is rotated by a certain degree :math:`\theta_0`, and thus the equation should be modified to take into account this:

.. math::

   \begin{pmatrix}
      x_e \\
      y_e
   \end{pmatrix}
   =
   \begin{pmatrix}
      \cos \theta_0 & -\sin \theta_0 \\
      \sin \theta_0 &  \cos \theta_0
   \end{pmatrix}
   \begin{pmatrix}
      a_0 \cos t_0 \\
      b_0 \sin t_0
   \end{pmatrix}.

Furthermore, the center :math:`( x_0, y_0 )` is not at the origin in general, which again modifies the equation as follows to obtain the final equation:

.. math::

   \begin{pmatrix}
      x_e \\
      y_e
   \end{pmatrix}
   =
   \begin{pmatrix}
      x_0 \\
      y_0
   \end{pmatrix}
   +
   \begin{pmatrix}
      \cos \theta_0 & -\sin \theta_0 \\
      \sin \theta_0 &  \cos \theta_0
   \end{pmatrix}
   \begin{pmatrix}
      a_0 \cos t_0 \\
      b_0 \sin t_0
   \end{pmatrix}.

The subscript :math:`0` is used to distinguish the two ellipses; namely, we obtain the same equation for the other ellipse by replacing :math:`0` with :math:`1`.

If we consider this problem naively, the collision of these two ellipses yields a quartic equation with respect to the intersecting point(s) :math:`x` (or :math:`y`).
The solution can be categorised as follows:

#. Four real solutions

   Four intersecting points

#. Two real and two imaginary solutions

   Two intersecting points

#. One real and two imaginary solutions

   Colliding at one point

#. Four imaginary solutions

   No Collision

==========
Difficulty
==========

Although we know a quartic formula, it is non-trivial to treat it numerically.
For instance, we need to implement the conditions very carefully to avoid NaN, which easily appears because of numerical errors involved.

Also, even if we obtain a theoretical solution, it is still unclear how to define the penetration depth :math:`\delta`.

Because of these two reasons, I decided to consider a simpler solution.

***********************************
Approximation by a circular fitting
***********************************

===========================================
Minimum distance from a point to an ellipse
===========================================

.. note::

   This part is largely inspired by `Simple Method for Distance to Ellipse <https://blog.chatfield.io/simple-method-for-distance-to-ellipse/>`_.

We first consider to find a minimum distance from a point :math:`( x_p, y_p )` to an ellipse, which is equivalent to find a normal vector from the point to the ellipse.
Moreover, this is the same thing to consider a circle (whose center is at :math:`( x_c, y_c )` and its radius is :math:`r`) approximating the ellipse locally.

An example can be found in the picture below, where one can see that:

* The black arrow connecting :math:`( x_p, y_p )` and :math:`( x_c, y_c )` gives a normal vector to the ellipse,

* Fitted circle gives a good approximation of the local curved nature of the original ellipse.

.. image:: data/fit-circle.png
   :width: 400

Whole procedures to draw the above schematic will be documented here.
First of all, we need to configure an ellipse:

.. myliteralinclude:: /../../src/fit_circle.c
   :language: c
   :tag: initialise an ellipse

Here prefixes ``e0`` are used to indicate that these variables are for *the ellispe of index 0* (you will find 1 in the next section).
The subscripts denote parameters which are necessary to uniquely determine the ellipse (``a`` and ``b``: major and minor axes, ``x`` and ``y``: center of ellipse, ``theta``: rotation angle in radian).

Also a point :math:`( x_p, y_p )` is declared and initialised, from which a minimum distance to an ellipse will be considered:

.. myliteralinclude:: /../../src/fit_circle.c
   :language: c
   :tag: set target point (xp, yp)

Since the ellipse is rotated and its center does not locate at the origin, we transform the coordinate system to simplify the problem.

.. myliteralinclude:: /../../src/fit_circle.c
   :language: c
   :tag: transform coordinate, forward

Here ``e0_xp_`` and ``e0_yp_`` are the target point :math:`( x_p, y_p )` after being transformed where the center lies at the origin and the ellipse is no longer rotated (:math:`\theta_0 = 0`).
Obviously we need to first move the ellipse to the center and rotate it, which is taken care of by a function ``shift_and_rotate``.

For this *simple* ellipse, the center of the fitted circle :math:`( x_c, y_c )` for a specific :math:`t` (same parameter used to describe the ellipse) leads

.. math::

   \left(
      x_c,
      y_c
   \right)
   =
   \left(
      a \left( 1 - \frac{b^2}{a^2} \right) \cos^3 t,
      b \left( 1 - \frac{a^2}{b^2} \right) \sin^3 t
   \right),

which is the so-called `evolute <https://en.wikipedia.org/wiki/Evolute#Evolute_of_an_ellipse>`_.
Also the local curvature is given by

.. math::

   \kappa
   =
   \frac{
     ab
   }{
      \sqrt{\left( a^2 \sin^2 t + b^2 \cos^2 t \right)^3}
   },

whose reciprocal is the radius of the fitted circle.

So, we want to find :math:`t` with which a vector from the center of the evolute ``(e0_xc_, e0_yc_)`` to the target point ``(e0_xp_, e0_yp_)`` gives a normal vector to the ellipse.
The question is: how?

This is answered by `the original project <https://blog.chatfield.io/simple-method-for-distance-to-ellipse/>`_ elegantly.
The methodology and the implementation in Python can be found in the above link, whose C version ``ellipse_find_normal_t`` is used here:

.. myliteralinclude:: /../../src/fit_circle.c
   :language: c
   :tag: find desired t

.. details:: ellipse_find_normal_t

   The definition of the function is as follows:

   .. code-block:: c

      double ellipse_find_normal_t(
          const double a,
          const double b,
          const double xp,
          const double yp
      );

   which is implemented in `src/ellipse.c <https://github.com/NaokiHori/Collision-of-Ellipse/blob/main/src/ellipse.c>`_.

   This takes the major (``a``) and minor (``b``) axes of the ellipse as well as the coordinate of the target point (``xp, yp``).
   The user should transform the coordinate so that the center of the ellipse locates at the origin and it is not rotated, i.e., the major axis is on the :math:`x` axis.

   Since the ellipse is not rotated and its center is at the origin, the whole things are symmetric with respect to the :math:`x` and :math:`y` axes.
   Thus we can assume that the target point ``(xp, yp)`` exists in the first quadrant:

   .. myliteralinclude:: /../../src/ellipse.c
      :language: c
      :tag: consider in the 1st quadrant

   Also the initial guess of the result ``t`` is given:

   .. myliteralinclude:: /../../src/ellipse.c
      :language: c
      :tag: initialise t

   Using this ``t``, we compute the point on the ellipse:

   .. myliteralinclude:: /../../src/ellipse.c
      :language: c
      :tag: compute point on the ellipse

   as well as the evolute (center of the fitted circle):

   .. myliteralinclude:: /../../src/ellipse.c
      :language: c
      :tag: compute center of the fitted circle

   Then ``dt``, which is the change of ``t`` to the desired value, is computed:

   .. myliteralinclude:: /../../src/ellipse.c
      :language: c
      :tag: compute residual

   See the original document for the details of this part.

   Finally ``t`` is updated:

   .. myliteralinclude:: /../../src/ellipse.c
      :language: c
      :tag: update t

   Note that ``t`` should be bounded between :math:`0` and :math:`\pi / 2` since we limit our evaluation inside the first quadrant.

   This procedure is iterated until the residual ``dt`` is small enough:

   .. myliteralinclude:: /../../src/ellipse.c
      :language: c
      :tag: terminate iteration when the residual is sufficiently small

   We need to return to the quadrant where the target point was in:

   .. myliteralinclude:: /../../src/ellipse.c
      :language: c
      :tag: recover information of the quadrants

   We go back to the client function by returning the resulting value:

   .. myliteralinclude:: /../../src/ellipse.c
      :language: c
      :tag: return final t

   .. note::
      Although it is an iterative method, about 5 iterations are sufficient to obtain a good approximation.
      Moreover, it is very robust and stable.

The final step is to go back to the original coordinate system:

.. myliteralinclude:: /../../src/fit_circle.c
   :language: c
   :tag: transform coordinate, backward

Here the center of the evolute is transformed to the original coordinate system ``(e0_xc, e0_yc)`` to draw the above sketch.
Obviously we need to first rotate the ellipse and later move the center to the original position, which is taken care of by a function ``rotate_and_shift``.

.. seealso::

   One can find the script which is used to draw the above schematic in `src/fit_circle.c <https://github.com/NaokiHori/Collision-of-Ellipse/blob/main/src/fit_circle.c>`_.

=========================
Collision of two ellipses
=========================

I use the above method to quantify the penetration depth :math:`\delta`.
In particular, for the ellipse :math:`0`, the center of the evolute of the ellipse :math:`1` :math:`( x_{c_1}, y_{c_1} )` is used as the target point :math:`( x_{p_0}, y_{p_0} )` to fit a circle, and vice versa for the ellipse :math:`1`.
This process is iterated until the locations of :math:`( x_{c_i}, y_{c_i} )` converge.

When the two ellipses are colliding, the fitting circles lead

.. image:: data/fit-circles-0.png
   :width: 400

When the two ellipses are not colliding, the final state leads

.. image:: data/fit-circles-1.png
   :width: 400

A script to generate these datasets can be found `here <https://github.com/NaokiHori/Collision-of-Ellipse/blob/main/src/fit_circles.c>`_.

Since we obtain circles, it is straightforward to define a penetration depth as

.. math::

   \delta \equiv r_0 + r_1 - d,

where :math:`r_i` are radii of the fitted circles, while :math:`d` is the distance between two centers (of the fitting circles).
It is readily apparent that we can conclude that two circles are colliding when :math:`\delta > 0`.
Moreover, this determination gives a good estimation even for the collision between ellipses.

