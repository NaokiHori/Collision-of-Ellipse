############################
Problem setup and difficulty
############################

*************
Problem setup
*************

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

1. Four real solutions

   Four intersecting points

2. Two real and two imaginary solutions

   Two intersecting points

3. One real and two imaginary solutions

   Colliding at one point

4. Four imaginary solutions

   No Collision

In particular, we are interested in the second and the fourth options.
The first one is out of focus since we are only interested in when the collision depth is fairly small, while the third one never happens when things are treated numerically.

**********
Difficulty
**********

Although we know a quartic formula, it is non-trivial to treat it numerically.
For instance, we need to implement the conditions very carefully to avoid NaN, which easily appears because of numerical errors involved.

Also, even if we obtain a theoretical solution, it is still unclear how to define the penetration depth :math:`\delta`.

Because of these two reasons, I decided to consider a simpler solution.

