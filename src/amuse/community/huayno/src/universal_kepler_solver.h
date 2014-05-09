#ifndef __UNIVERSAL_KEPLER_SOLVER_H__
#define __UNIVERSAL_KEPLER_SOLVER_H__

#include "common.h"


static inline REAL stumpff_c0(
//    const REAL zeta)
    REAL zeta)      // This is because of bug 4775 on old versions of glibc.
                    // That bug has already been fixed in latter versions.
{
    REAL abs_zeta = fabs(zeta);
    if (abs_zeta < 1) {
        /* Taylor series: 1 + z / 2 + z*z / 24 + z*z*z / 720 + ...
         *
         * Horner form: ((... + 1) * z / 12 + 1) * z / 2 + 1
         *
         * The i-th coefficient for Horner form is generated by:
         *
         * a_i = (2*i+2)! / (2*i)!
         */
        return ((((((((((
          zeta / 380 + 1)
        * zeta / 306 + 1)
        * zeta / 240 + 1)
        * zeta / 182 + 1)
        * zeta / 132 + 1)
        * zeta / 90  + 1)
        * zeta / 56  + 1)
        * zeta / 30  + 1)
        * zeta / 12  + 1)
        * zeta / 2   + 1);
    }

    REAL sz = sqrt(abs_zeta);
    if (zeta < 0) {
        return cos(sz);
    }
    return cosh(sz);
}


static inline REAL stumpff_c1(
//    const REAL zeta)
    REAL zeta)
{
    REAL abs_zeta = fabs(zeta);
    if (abs_zeta < 1) {
        /* Taylor series: 1 + z / 6 + z*z / 120 + z*z*z / 5040 + ...
         *
         * Horner form: ((... + 1) * z / 20 + 1) * z / 6 + 1
         *
         * The i-th coefficient for Horner form is generated by:
         *
         * a_i = (2*i+3)! / (2*i+1)!
         */
        return ((((((((((
              zeta / 420 + 1)
            * zeta / 342 + 1)
            * zeta / 272 + 1)
            * zeta / 210 + 1)
            * zeta / 156 + 1)
            * zeta / 110 + 1)
            * zeta / 72  + 1)
            * zeta / 42  + 1)
            * zeta / 20  + 1)
            * zeta / 6   + 1);
    }

    REAL sz = sqrt(abs_zeta);
    if (zeta < 0) {
        return sin(sz) / sz;
    }
    return sinh(sz) / sz;
}


static inline REAL stumpff_c2(
//    const REAL zeta)
    REAL zeta)
{
    REAL abs_zeta = fabs(zeta);
    if (abs_zeta < 1) {
        /* stumpff_c2(z) = (stumpff_c0(z) - 1) / z
         */
        return (((((((((
              zeta / 380 + 1)
            * zeta / 306 + 1)
            * zeta / 240 + 1)
            * zeta / 182 + 1)
            * zeta / 132 + 1)
            * zeta / 90  + 1)
            * zeta / 56  + 1)
            * zeta / 30  + 1)
            * zeta / 12  + 1) / 2;
    }

    REAL sz = sqrt(abs_zeta);
    if (zeta < 0) {
        return (cos(sz) - 1) / zeta;
    }
    return (cosh(sz) - 1) / zeta;
}


static inline REAL stumpff_c3(
//    const REAL zeta)
    REAL zeta)
{
    REAL abs_zeta = fabs(zeta);
    if (abs_zeta < 1) {
        /* stumpff_c3(z) = (stumpff_c1(z) - 1) / z
         */
        return (((((((((
              zeta / 420 + 1)
            * zeta / 342 + 1)
            * zeta / 272 + 1)
            * zeta / 210 + 1)
            * zeta / 156 + 1)
            * zeta / 110 + 1)
            * zeta / 72  + 1)
            * zeta / 42  + 1)
            * zeta / 20  + 1) / 6;
    }

    REAL sz = sqrt(abs_zeta);
    if (zeta < 0) {
        return (sin(sz) / sz - 1) / zeta;
    }
    return (sinh(sz) / sz - 1) / zeta;
}


static inline REAL S0(
    const REAL s,
    const REAL alpha)
{
    REAL s2 = s * s;
    REAL zeta = alpha * s2;
    return stumpff_c0(zeta);
}


static inline REAL S1(
    const REAL s,
    const REAL alpha)
{
    REAL s2 = s * s;
    REAL zeta = alpha * s2;
    return s * stumpff_c1(zeta);
}


static inline REAL S2(
    const REAL s,
    const REAL alpha)
{
    REAL s2 = s * s;
    REAL zeta = alpha * s2;
    return s2 * stumpff_c2(zeta);
}


static inline REAL S3(
    const REAL s,
    const REAL alpha)
{
    REAL s2 = s * s;
    REAL s3 = s * s2;
    REAL zeta = alpha * s2;
    return s3 * stumpff_c3(zeta);
}


static inline REAL lagrange_f(
    const REAL s,
    const REAL r0,
    const REAL m,
    const REAL alpha)
{
    return 1 - m * S2(s, alpha) / r0;
}


static inline REAL lagrange_g(
    const REAL s,
    const REAL r0,
    const REAL r0v0,
    const REAL alpha)
{
//    return r0 * s + r0v0 * S2(s, alpha) + (alpha * r0) * S3(s, alpha);
    return r0 * S1(s, alpha) + r0v0 * S2(s, alpha);
}


static inline REAL lagrange_dfds(
    const REAL s,
    const REAL r0,
    const REAL m,
    const REAL alpha)
{
    return -m * S1(s, alpha) / r0;
}


static inline REAL lagrange_dgds(
    const REAL s,
    const REAL r0,
    const REAL r0v0,
    const REAL alpha)
{
//    return r0 + r0v0 * S1(s, alpha) + (alpha * r0) * S2(s, alpha);
    return r0 * S0(s, alpha) + r0v0 * S1(s, alpha);
}


static inline REAL universal_kepler(
    const REAL s,
    const REAL r0,
    const REAL r0v0,
    const REAL m,
    const REAL alpha)
{
//    return r0 * S1(s, alpha) + r0v0 * S2(s, alpha) + m * S3(s, alpha);
    return r0 * s + r0v0 * S2(s, alpha) + (m + alpha * r0) * S3(s, alpha);
}


static inline REAL universal_kepler_ds(
    const REAL s,
    const REAL r0,
    const REAL r0v0,
    const REAL m,
    const REAL alpha)
{
//    return r0 * S0(s, alpha) + r0v0 * S1(s, alpha) + m * S2(s, alpha);
    return r0 + r0v0 * S1(s, alpha) + (m + alpha * r0) * S2(s, alpha);
}


static inline REAL universal_kepler_dsds(
    const REAL s,
    const REAL r0,
    const REAL r0v0,
    const REAL m,
    const REAL alpha)
{
//    return r0v0 * S0(s, alpha) + (m + alpha * r0) * S1(s, alpha);
    return r0v0 * S0(s, alpha) + (m + alpha * r0) * S1(s, alpha);
}


static inline REAL f(
    const REAL s,
    REAL *arg)
{
    return universal_kepler(s, arg[1], arg[2], arg[3], arg[4]) - arg[0];
}


static inline REAL fprime(
    const REAL s,
    REAL *arg)
{
    return universal_kepler_ds(s, arg[1], arg[2], arg[3], arg[4]);
}


static inline REAL fprimeprime(
    const REAL s,
    REAL *arg)
{
    return universal_kepler_dsds(s, arg[1], arg[2], arg[3], arg[4]);
}


#define ORDER 5
static inline REAL laguerre(
    const REAL s,
    REAL *arg)
{
    REAL fv = f(s, arg);
    REAL dfv = fprime(s, arg);
    REAL ddfv = fprimeprime(s, arg);
    REAL a = dfv;
    REAL a2 = a * a;
    REAL b = a2 - fv * ddfv;
    REAL g = ORDER * fv;
    REAL h = a + SIGN(a) * sqrt(fabs((ORDER - 1) * (ORDER * b - a2)));

    return -g / h;
}


static inline REAL halley(
    const REAL s,
    REAL *arg)
{
    REAL fv = f(s, arg);
    REAL dfv = fprime(s, arg);
    REAL ddfv = fprimeprime(s, arg);
    REAL g = 2 * fv * dfv;
    REAL h = (2 * dfv * dfv - fv * ddfv);

    return -g / h;
}


static inline REAL newton(
    const REAL s,
    REAL *arg)
{
    REAL fv = f(s, arg);
    REAL dfv = fprime(s, arg);
    REAL g = fv;
    REAL h = dfv;

    return -g / h;
}


static inline REAL fdelta(
    const REAL s,
    REAL *arg)
{
//    return newton(s, arg);
//    return halley(s, arg);
    return laguerre(s, arg);
}


static inline INT findroot(
    REAL x0,
    REAL *x,
    REAL *arg)
{
    INT n = 0;
    REAL a, b;

    *x = x0;
    do {
        REAL delta = fdelta(*x, arg);
        if (fabs(delta) > fabs(*x))
            delta = SIGN(delta) * fabs((*x)/2);

        a = (*x);
        (*x) += delta;
        b = (*x);

        if (n > MAXITER) return -1;
        n += 1;
    } while (2 * fabs(b - a) > TOLERANCE * fabs(a + b));

    return 0;
}


static inline void set_new_pos_vel(
    const REAL s,
    const REAL r0,
    const REAL r0v0,
    const REAL m,
    const REAL e2,
    const REAL alpha,
    const REAL r0x,
    const REAL r0y,
    const REAL r0z,
    const REAL v0x,
    const REAL v0y,
    const REAL v0z,
    REAL *r1x,
    REAL *r1y,
    REAL *r1z,
    REAL *v1x,
    REAL *v1y,
    REAL *v1z)
{
    REAL lf = lagrange_f(s, r0, m, alpha);
    REAL lg = lagrange_g(s, r0, r0v0, alpha);

    *r1x = r0x * lf + v0x * lg;
    *r1y = r0y * lf + v0y * lg;
    *r1z = r0z * lf + v0z * lg;

    REAL ldf = lagrange_dfds(s, r0, m, alpha);
    REAL ldg = lagrange_dgds(s, r0, r0v0, alpha);

    REAL r1 = lf * ldg - lg * ldf;

    ldf /= r1;
    ldg /= r1;

    *v1x = r0x * ldf + v0x * ldg;
    *v1y = r0y * ldf + v0y * ldg;
    *v1z = r0z * ldf + v0z * ldg;
}


static inline INT __universal_kepler_solver(
    const REAL dt0,
    const REAL m,
    const REAL e2,
    const REAL r0x,
    const REAL r0y,
    const REAL r0z,
    const REAL v0x,
    const REAL v0y,
    const REAL v0z,
    REAL *r1x,
    REAL *r1y,
    REAL *r1z,
    REAL *v1x,
    REAL *v1y,
    REAL *v1z)
{
    *r1x = r0x;
    *r1y = r0y;
    *r1z = r0z;
    *v1x = v0x;
    *v1y = v0y;
    *v1z = v0z;

    REAL r0sqr = r0x * r0x + r0y * r0y + r0z * r0z;
    if (!(r0sqr > 0)) return 0;

    r0sqr += e2;
    REAL r0 = sqrt(r0sqr);

    REAL r0v0 = r0x * v0x + r0y * v0y + r0z * v0z;
    REAL v0sqr = v0x * v0x + v0y * v0y + v0z * v0z;
    REAL u0sqr = 2 * m / r0;
    REAL u = sqrt(u0sqr);
    REAL v = sqrt(v0sqr);
//    REAL alpha0 = v0sqr - u0sqr;
    REAL alpha0 = (v - u) * (v + u);
    REAL lagr0 = v0sqr + u0sqr;
    REAL abs_alpha0 = fabs(alpha0);

    #ifndef CONFIG_USE_OPENCL
    if (r0 * abs_alpha0 < 32 * sqrt(TOLERANCE) * m) {
        fprintf(stderr, "#---WARNING: Floating point precision "
                        "used may be lower than required.\n");
        fprintf(stderr, "#---err flag: None\n");
        fprintf(stderr,
            "#   dt0: %a, m: %a, e2: %a,"
            " r0x: %a, r0y: %a, r0z: %a,"
            " v0x: %a, v0y: %a, v0z: %a\n"
            "#   r0: %a, r0v0: %a, v0sqr: %a,"
            " u0sqr: %a, alpha0: %a\n",
            dt0, m, e2,
            r0x, r0y, r0z,
            v0x, v0y, v0z,
            r0, r0v0, v0sqr,
            u0sqr, alpha0);
        fprintf(stderr, "#---\n");
    }
    #endif

    REAL s0, s, arg[5];

    REAL dt = dt0;

    /* First guess for highly hyperbolic orbits:
     * adapted from formula 4.5.11 in fundamentals
     * of astrodynamics (Bate et al. 1971).
     */
    REAL ss = (2 * alpha0 * fabs(dt0 / (r0v0 + (m + alpha0 * r0) / sqrt(abs_alpha0))));
    if (ss > 1) {
        s0 = SIGN(dt0) * log(ss) / sqrt(abs_alpha0);
    } else {
        /* For elliptical orbits: reduce the time
         * step to a fraction of the orbital period.
         */
        if (alpha0 < 0) {
            REAL T = 2 * PI * m / (abs_alpha0 * sqrt(abs_alpha0));
            REAL ratio = dt0 / T;
            dt = (ratio - (INT)(ratio)) * T;
        }
        /* This seems to work well for both
         * elliptical and nearly parabolical
         * orbits.
         */
        s0 = dt * abs_alpha0 / m;

        REAL s01 = dt / r0;
        if (fabs(alpha0 * s01 * s01) < 1)
            s0 = s01;
    }

    arg[0] = dt;
    arg[1] = r0;
    arg[2] = r0v0;
    arg[3] = m;
    arg[4] = alpha0;

    INT err = findroot(s0, &s, arg);
    if (err != 0) {
        #ifndef CONFIG_USE_OPENCL
        fprintf(stderr, "#---WARNING: Maximum iteration steps "
                        "reached in 'findroot' function. Trying "
                        "again with two steps of size dt0/2.\n");
        fprintf(stderr, "#---err flag: %ld\n", (long)(err));
        fprintf(stderr,
            "#   dt0: %a, m: %a, e2: %a,"
            " r0x: %a, r0y: %a, r0z: %a,"
            " v0x: %a, v0y: %a, v0z: %a\n"
            "#   dt: %a, r0: %a, r0v0: %a, v0sqr: %a, u0sqr: %a,"
            " alpha0: %a, s0: %a, s: %a, ss: %a\n",
            dt0, m, e2,
            r0x, r0y, r0z,
            v0x, v0y, v0z,
            dt, r0, r0v0, v0sqr, u0sqr,
            alpha0, s0, s, ss);
        fprintf(stderr, "#---\n");
        #endif
        return err;
    }

    REAL alpha = alpha0;
    if (e2 > 0) {
        REAL r1 = universal_kepler_ds(s, r0, r0v0, m, alpha);
        REAL inv_r = (1 / r0 + 1 / r1) / 2;
        alpha = alpha0 + m * e2 * inv_r * inv_r * inv_r;
        r1 = universal_kepler_ds(s, r0, r0v0, m, alpha);
        inv_r = (1 / r0 + 1 / r1) / 2;
        alpha = alpha0 + m * e2 * inv_r * inv_r * inv_r;
    }

    set_new_pos_vel(s, r0, r0v0, m, e2, alpha,
                    r0x, r0y, r0z, v0x, v0y, v0z,
                    &(*r1x), &(*r1y), &(*r1z),
                    &(*v1x), &(*v1y), &(*v1z));

    if (e2 > 0) {
        REAL r1sqr = *r1x * *r1x + *r1y * *r1y + *r1z * *r1z;
        r1sqr += e2;
        REAL r1 = sqrt(r1sqr);
        REAL v1sqr = *v1x * *v1x + *v1y * *v1y + *v1z * *v1z;
        REAL u1sqr = 2 * m / r1;
        REAL lagr1 = v1sqr + u1sqr;
        REAL alpha1 = v1sqr - u1sqr;
        if (fabs(alpha1 - alpha0) > TOLERANCE * fabs(lagr1 + lagr0)) return -11;
    }

    return err;
}


static inline INT _universal_kepler_solver(
    const REAL dt,
    const REAL m,
    const REAL e2,
    const REAL r0x,
    const REAL r0y,
    const REAL r0z,
    const REAL v0x,
    const REAL v0y,
    const REAL v0z,
    REAL *r1x,
    REAL *r1y,
    REAL *r1z,
    REAL *v1x,
    REAL *v1y,
    REAL *v1z)
{
    INT err = __universal_kepler_solver(dt, m, e2,
                                        r0x, r0y, r0z,
                                        v0x, v0y, v0z,
                                        &(*r1x), &(*r1y), &(*r1z),
                                        &(*v1x), &(*v1y), &(*v1z));
    if (err == 0) return err;

    INT n = 2;
    do {
        err = 0;
        *r1x = r0x;
        *r1y = r0y;
        *r1z = r0z;
        *v1x = v0x;
        *v1y = v0y;
        *v1z = v0z;
        for (INT i = 0; i < n; ++i) {
            err |= __universal_kepler_solver(dt/n, m, e2,
                                             *r1x, *r1y, *r1z,
                                             *v1x, *v1y, *v1z,
                                             &(*r1x), &(*r1y), &(*r1z),
                                             &(*v1x), &(*v1y), &(*v1z));
        }
        if (n > MAXITER * MAXITER) return err;
        n *= 2;
    } while (err != 0);

    return err;
}


static inline INT universal_kepler_solver(
    const REAL dt,
    const REAL m,
    const REAL e2,
    const REAL r0x,
    const REAL r0y,
    const REAL r0z,
    const REAL v0x,
    const REAL v0y,
    const REAL v0z,
    REAL *r1x,
    REAL *r1y,
    REAL *r1z,
    REAL *v1x,
    REAL *v1y,
    REAL *v1z)
{
    INT err = _universal_kepler_solver(dt, m, e2,
                                       r0x, r0y, r0z,
                                       v0x, v0y, v0z,
                                       &(*r1x), &(*r1y), &(*r1z),
                                       &(*v1x), &(*v1y), &(*v1z));
    #ifndef CONFIG_USE_OPENCL
    if (err != 0) {
        fprintf(stderr, "#---ERROR: The solution "
                        "may not have converged.\n");
        fprintf(stderr, "#---err flag: %ld\n", (long)(err));
        fprintf(stderr,
            "#   dt: %a, m: %a, e2: %a,"
            " r0x: %a, r0y: %a, r0z: %a,"
            " v0x: %a, v0y: %a, v0z: %a\n",
            dt, m, e2,
            r0x, r0y, r0z,
            v0x, v0y, v0z);
        fprintf(stderr, "#---\n");
    }
    #endif

    return err;
}

#endif  // __UNIVERSAL_KEPLER_SOLVER_H__