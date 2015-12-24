#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <memory.h>
#include <stdio.h>

typedef double  f_t;
typedef f_t			v_t[4];
typedef v_t			m_t[4];
typedef unsigned char			iter_t;


static void
v_zero(	v_t   c)
{
	memset( c, 0, sizeof(v_t) );
}


static void
v_add(
	v_t 			c,
	const v_t 		a,
	const v_t 		b,
	iter_t			n
)
{
	do {
		--n;
		c[n] = a[n] + b[n];
	} while( n > 0 );
}


static void
v_sub(
	v_t 			c,
	const v_t 		a,
	const v_t 		b,
	iter_t			n
)
{
	do {
		--n;
		c[n] = a[n] - b[n];
	} while( n > 0 );
}


static void
v_scale(
	v_t			c,
	const v_t		a,
	f_t			s,
	iter_t			n
)
{
	do {
		--n;
		c[n] = a[n] * s;
	} while( n > 0 );
}



/*
 *  Matrix operations
 */
static void
m_zero(
	m_t			c
)
{
	memset( c, 0, sizeof(m_t) );
}


/*
 * Adds two matrices: c = a + b
 *
 * Safe to call with c == a and c == b
 * Requires 2300 instructions (580 useconds)
 */
static void
m_add(
	m_t 			c,
	m_t 			a,
	m_t			b,
	iter_t			m,
	iter_t			n
)
{
	do {
		n--;
		v_add( c[n], a[n], b[n], m );
	} while( n > 0 );
}


static void
m_sub(
	m_t 			c,
	m_t 			a,
	m_t			b,
	iter_t			m,
	iter_t			n
)
{
	do {
		n--;
		v_sub( c[n], a[n], b[n], m );
	} while( n > 0 );
}


static void
m_scale(
	m_t			c,
	m_t			a,
	f_t			s,
	iter_t			m,
	iter_t			n
)
{
	do {
		n--;
		v_scale( c[n], a[n], s, m );
	} while( n > 0 );
}



/**
 *  vc = ma * vb
 *
 * Unsafe to call with vc == vb
 */
static void
mv_mult(
	v_t			vc,
	m_t			ma,
	const v_t		vb,
	iter_t			m,
	iter_t			n
)
{
	iter_t			i;
	iter_t			j;

	for( j=0 ; j<m ; ++j )
	{
		f_t			sum = (f_t) 0;
		const f_t *		row = &ma[j][0];

		for( i=0 ; i<n ; ++i )
			sum += row[i] * vb[i];

		vc[j] = sum;
	}
}


/**
 *  vc = va * mb
 *
 * Unsafe to call with vc == va
 */
static void
vm_mult(
	v_t			vc,
	v_t			va,
	m_t			mb,
	iter_t			m,
	iter_t			n
)
{
	iter_t			i;
	iter_t			j;

	for( i=0 ; i<n ; ++i )
	{
		f_t			sum = (f_t) 0;

		for( j=0 ; j<m ; j++ )
			sum += va[j] * mb[j][i];

		vc[j] = sum;
	}
}


/**
 *  c = a * b
 *
 * Unsafe for c == a or c == b.
 *
 * Optimal form runs in 1/2 the time of the simple form due to
 * loop constant hoisting and others.
 *
 * Requires 14630 instructions (3650 usec)
 */
static void
m_mult(
	m_t			c,
	m_t			a,
	m_t			b,
	iter_t			m,
	iter_t			n,
	iter_t			p
)
{
	iter_t			i;
	iter_t			j;
	iter_t			k;

#ifdef SIMPLE
	m_zero( c );

	for( i=0 ; i<m ; ++i )
		for( j=0 ; j<p ; ++j )
			for( k=0 ; k<n ; ++k )
				c[i][j] += a[i][k] * b[k][j];
#else
	for( i=0 ; i<m ; ++i )
	{
		for( j=0 ; j<p ; ++j )
		{
			f_t		sum = (f_t) 0;
			const f_t *	A = &a[i][0];

			for( k=0 ; k<n ; ++k )
				sum += A[k] * b[k][j];
			c[i][j] = sum;
		}
	}
#endif

}


/**
 *  transpose is safe to call with c == a
 */
static void
m_transpose(
	m_t			c,
	m_t			a,
	iter_t			m,
	iter_t			n
)
{
	iter_t			i;
	iter_t			j;

	for( i=0 ; i<m ; ++i )
	{
		for( j=i ; j<n ; j++ )
		{
			f_t			u = a[i][j];
			f_t			l = a[j][i];

			c[j][i] = u;
			c[i][j] = l;
		}
	}
}


/*
 *  Produce the determinant of a 3x3 matrix
 */
static f_t
m33_det(
	m_t			m
)
{
	return	m[0][0] * m[1][1] * m[2][2]
	- 	m[0][0] * m[1][2] * m[2][1]
	-	m[0][1] * m[1][0] * m[2][2]
	+	m[0][1] * m[1][2] * m[2][0]
	+	m[0][2] * m[1][0] * m[2][1]
	-	m[0][2] * m[1][1] * m[2][0]
	;
}



/**
 *  m33_inv will invert a 3x3 matrix.
 *
 * Unsafe to call with c == a
 *
 * Details were copied from:
 *
 *	http://mathworld.wolfram.com/MatrixInverse.html
 *
 * Requires 16478 instructions (4120 useconds)
 */
static void
m33_inv(
	m_t			D,
	m_t			m
)
{
	D[0][0] = m[1][1] * m[2][2] - m[1][2] * m[2][1];
	D[0][1] = m[0][2] * m[2][1] - m[0][1] * m[2][2];
	D[0][2] = m[0][1] * m[1][2] - m[0][2] * m[1][1];

	D[1][0] = m[1][2] * m[2][0] - m[1][0] * m[2][2];
	D[1][1] = m[0][0] * m[2][2] - m[0][2] * m[2][0];
	D[1][2] = m[0][2] * m[1][0] - m[0][0] * m[1][2];

	D[2][0] = m[1][0] * m[2][1] - m[1][1] * m[2][0];
	D[2][1] = m[0][1] * m[2][0] - m[0][0] * m[2][1];
	D[2][2] = m[0][0] * m[1][1] - m[0][1] * m[1][0];

	m_scale( D, D, 1.0 / m33_det(m), 3, 3 );
}


static void
m_print(
	const char *		label,
	m_t			a,
	iter_t			m,
	iter_t			n
)
{
	iter_t			i;
	iter_t			j;

	printf( "%s=", label );
	for( i=0 ; i<m ; i++ )
	{
		printf( "[" );
		for( j=0 ; j<n ; j++ )
			printf( " %lf", a[i][j] );
		printf( "]\n" );
	}
}

#endif
