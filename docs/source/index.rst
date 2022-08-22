####################
Collision-of-Ellipse
####################

**********
Motivation
**********

I was interested in the collisions between two ellipses when I developed `this library <https://github.com/NaokiHori/SimpleSolidSuspensionSolver>`_.

Namely, under this kind of circumstance,

.. image:: data/problem-setup.png
   :width: 400

I needed to consider

* whether they are overlapped or not,
* if they are, quantification of the penetration depth :math:`\delta`.

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

In general, an ellipse is rotated to a certain degree :math:`\theta_0`, and thus the equation should be modified to take into account this:

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

Furthermore, the center :math:`( x_0, y_0 )` is not at the origin, which again modifies the equation as follows to obtain the final equation:

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

Although we know a quartic formula, it is non-trivial to treat numerically.
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

We first consider to find a minimum distance from a point :math:`( x_p, y_p )` to an ellipse, which is equivalent to consider a circle (whose center is at :math:`( x_c, y_c )` and its radius is :math:`r`) approximating the ellipse locally.

When the ellipse is not rotated and its center is at the origin, the fitting circle for a variable :math:`t` leads

.. math::

   \begin{pmatrix}
      x_c \\
      y_c
   \end{pmatrix}
   =
   \begin{pmatrix}
      a \left( 1 - \frac{b^2}{a^2} \right) \cos^3 t \\
      b \left( 1 - \frac{a^2}{b^2} \right) \sin^3 t
   \end{pmatrix},

which is the so-called evolute and its local curvature is given by

.. math::

   \kappa
   =
   \frac{
     ab
   }{
      \sqrt{\left( a^2 \sin^2 t + b^2 \cos^2 t \right)^3}
   }.

An example can be found in the picture below, where one can see that the black arrow connecting two points :math:`( x_p, y_p )` and :math:`( x_c, y_c )` gives a tangential vector to the ellipse and the fitted circle.

.. image:: data/fit-circle.png
   :width: 400

Although it is an iterative method, about 5 iterations are sufficient to obtain a good approximation.
Moreover, it is very robust and stable.

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

Since we obtain circles, it is straightforward to define a penetration depth as

.. math::

   \delta \equiv r_0 + r_1 - d,

where :math:`r_i` are radii of the fitted circles, while :math:`d` is the distance between two centers.
We can conclude that two circles (or ellipses) are colliding when :math:`\delta > 0`.

