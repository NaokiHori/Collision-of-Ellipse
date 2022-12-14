###########################################
Collision evaluation with circular fittings
###########################################

*********************
Coordinate transforms
*********************

Although ellipses can rotate and their centers are not at the origin in general, it is cumbersome to discuss based on this general configuration.
Thus, hereafter, we consider to transform the coordinate system:

.. math::
   \begin{pmatrix}
     x_p \\
     y_p
   \end{pmatrix}
   \leftarrow
   \begin{pmatrix}
     \cos ( -\theta) & - \sin ( -\theta) \\
     \sin ( -\theta) &   \cos ( -\theta)
   \end{pmatrix}
   \begin{pmatrix}
     x_p-x_0 \\
     y_p-y_0
   \end{pmatrix}

so that we move to a new coordinate system in which the ellipse (whose center is at :math:`( x_0, y_0 )` and the rotation angle is :math:`\theta`) locates at the origin and does not rotate.

Also a position computed in this comfortable system should be transformed back

.. math::
   \begin{pmatrix}
     x_p \\
     y_p
   \end{pmatrix}
   \leftarrow
   \begin{pmatrix}
     x_0 \\
     y_0
   \end{pmatrix}
   +
   \begin{pmatrix}
     \cos ( +\theta) & - \sin ( +\theta) \\
     \sin ( +\theta) &   \cos ( +\theta)
   \end{pmatrix}
   \begin{pmatrix}
     x_p \\
     y_p
   \end{pmatrix}

to recover the information in the original system.

Hereafter, we assume that these treatments are properly done a priori and a posteriori.

***************************************
Approximation of an ellipse by a circle
***************************************

Although the collision between two ellipses is fairly complicated, the one between circles is much easier.
For instance, the penetration depth of two circles lead

.. math::

   \delta \equiv r_0 + r_1 - d,

where radii of the circles are :math:`r_0` and :math:`r_1` and its distance from center to center is given by :math:`d`.
I am interested in a relatively small collisions now; namely, :math:`\delta` is much smaller than the sizes of the colliding objects.
Thus, I concluded that considering the collision between two circles which approximate the ellipses in the vicinity of the colliding points is justified.

Luckily, an analytical relation exists for a circle which approximates an ellipse locally, whose center leads

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

So the question is: how can I compute :math:`t` which gives a circle nicely approximating the ellipse locally?

*******************************************
Minimum distance from a point to an ellipse
*******************************************

.. note::

   This part is largely inspired by `Simple Method for Distance to Ellipse <https://blog.chatfield.io/simple-method-for-distance-to-ellipse/>`_.

Actually, the above question is equivalent to find a normal vector from the point :math:`( x_p, y_p )` to the ellipse, and also is the same thing to consider the minimum distance to the ellipse.

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
Obviously, we need to first move the ellipse to the center and rotate it, which is taken care of by a function ``shift_and_rotate``.

Now we want to find :math:`t` with which a vector from the evolute ``(e0_xc_, e0_yc_)`` (i.e., center of the fitted circle) to the target point ``(e0_xp_, e0_yp_)`` gives a normal vector to the ellipse, which is solved by `the original project <https://blog.chatfield.io/simple-method-for-distance-to-ellipse/>`_ elegantly.
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

Here the center of the fitted circle (evolute) is transformed to the original coordinate system ``(e0_xc, e0_yc)`` to draw the above sketch.
Obviously, we need to first rotate the ellipse and later move the center to the original position, which is taken care of by a function ``rotate_and_shift``.

.. seealso::

   One can find the script which is used to draw the above schematic in `src/fit_circle.c <https://github.com/NaokiHori/Collision-of-Ellipse/blob/main/src/fit_circle.c>`_.

*************************
Collision of two ellipses
*************************

I use the above method to quantify the penetration depth :math:`\delta`.
In particular, for the ellipse :math:`0`, the center of the fitted circle of the ellipse :math:`1` :math:`( x_{c_1}, y_{c_1} )` is used as the target point :math:`( x_{p_0}, y_{p_0} )` to fit a circle, and vice versa for the ellipse :math:`1`.
Please refer to the previous section for the meaning of the terminologies (e.g., target points).
This process is iterated until the locations of :math:`( x_{c_i}, y_{c_i} )` converge.

When the two ellipses are colliding, the fitting circles lead

.. image:: data/fit-circles-0.png
   :width: 400

When the two ellipses are not colliding, the final state leads

.. image:: data/fit-circles-1.png
   :width: 400

The whole procedure to generate these sketches are as follows.

First of all, axes and the rotation angles of the two ellipses are prescribed:

.. myliteralinclude:: /../../src/fit_circles.c
   :language: c
   :tag: initialise geometry of ellipses

Since I want to draw two cases when two ellipses do and do not collide, two different positions of the ellipses are considered:

.. myliteralinclude:: /../../src/fit_circles.c
   :language: c
   :tag: initialise centers of ellipses

Here a parameter ``index`` is used to distinguish colliding (``0``) and not-colliding (``1``) cases.

Although we want to use the center of the *fitted circle* (evolute) of the opponent as the target point, it is not known at the beginning.
Here I use the coordinate of the center of the *ellipse itself* as a good approximation of the evolute to initialise ``(e0_xc, e0_yc)`` and ``(e1_xc, e1_yc)``:

.. myliteralinclude:: /../../src/fit_circles.c
   :language: c
   :tag: initialise evolutes using the centers of ellipses

Since all essential variables are given now, we can iterate the method discussed in the previous section for two ellipses until the parameters ``e0_t`` (:math:`t_0`) and ``e1_t`` (:math:`t_1`) converge; namely, we first transform coordinates of the centers of the fitted circles of the opponents (different coordinate is used for each ellipse):

.. myliteralinclude:: /../../src/fit_circles.c
   :language: c
   :tag: transform coordinates, forward

which is followed by the optimisations of the parameters (``e0_t`` and ``e1_t``):

.. myliteralinclude:: /../../src/fit_circles.c
   :language: c
   :tag: find desired t

and compute the centers of the corresponding fitted circles:

.. myliteralinclude:: /../../src/fit_circles.c
   :language: c
   :tag: update center of fitted circles

and finally recover the original coordinates by the backward transformations:

.. myliteralinclude:: /../../src/fit_circles.c
   :language: c
   :tag: transform coordinates, backward

.. seealso::

   One can find the script which is used to draw the above schematics in `src/fit_circles.c <https://github.com/NaokiHori/Collision-of-Ellipse/blob/main/src/fit_circles.c>`_.

Since we obtain circles, it is straightforward to define a penetration depth as

.. math::

   \delta \equiv r_0 + r_1 - d,

where :math:`r_i` are radii of the fitted circles, while :math:`d` is the distance between the two centers of the fitting circles.
It is readily apparent that we can conclude that two circles are colliding when :math:`\delta > 0`, and this can give a good estimation even for the collision depth between ellipses.

