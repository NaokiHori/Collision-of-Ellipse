####################
Collision-of-Ellipse
####################

I was interested in the collisions between two ellipses when I developed `this library <https://github.com/NaokiHori/SimpleSolidSuspensionSolver>`_.

Namely, under this kind of circumstance,

.. image:: data/problem-setup.png
   :width: 400

I needed to consider

* whether they are overlapped or not,
* if they are, how much the penetration depth :math:`\delta` is.

I assume the ellipses are completely rigid (no deformation), and :math:`\delta` is sufficiently small compared to the characteristic sizes of the colliding ellipses.

.. note::

   This is a compromised solution to obtain something *plausible* in my fluid-structure couplings.
   Namely, neither the detection nor the quantification of :math:`\delta` are rigorous and thus its application is limited.

.. seealso::

   `Documentation for Japanese readers <https://qiita.com/NaokiHori/items/daf3fd191d51a7e682f8>`_.

.. toctree::
   :maxdepth: 1

   setup
   main

