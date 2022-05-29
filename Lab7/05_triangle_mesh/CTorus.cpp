#include "stdafx.h"
#include "CTorus.h"
#include "Ray.h"
#include "Intersection.h"
#include "algebra.h"

CTorus::CTorus(double radius, double tubeRadius, CVector3d const& center, CMatrix4d const& transform)
	:CGeometryObjectWithInitialTransformImpl(transform)
	
{
	// Сфера заданного радиуса и с центром в заданной точке получается
	// путем масштабирования и переносы базовой сферы (сфера радиуса 1 с центром в начале координат)
	m_radius = radius;
	m_tubeRadius = tubeRadius;
	CMatrix4d initialTransform;
	initialTransform.Translate(center.x, center.y, center.z);
	// Задаем начальную трансформацию базовой сферы
	SetInitialTransform(initialTransform);
}

bool CTorus::Hit(CRay const& ray, CIntersection& intersection)const
{
	// Вместо преобразования сферы выполняем обратное преобразование луча
	// Результат будет тот же самый, но вычислить его будет проще
	CRay invRay = Transform(ray, GetInverseTransform());

	double solutions[4];
	int numSolutions = SolveIntersections(invRay.GetDirection(), invRay.GetStart(), solutions);

	double surfaeceTolerance = 1.0e-4;
	double t = DBL_MAX;
	bool intersected = false;
	for (int i = 0; i < numSolutions; i++)
	{
		if (solutions[i] > surfaeceTolerance) {
			intersected = true;
			if (solutions[i] < t)
				t = solutions[i];
		}
	}
	if (intersected)
	{
		CVector3d hitPoint = ray.GetPointAtTime(t);
		CVector3d hitPointInObjectSpace = invRay.GetPointAtTime(t);
		CVector3d const& hitNormalInObjectSpace = hitPointInObjectSpace;
		CVector3d hitNormal = GetNormalMatrix() * hitNormalInObjectSpace;
		CHitInfo hit(
			t, *this,
			hitPoint, hitPointInObjectSpace,
			hitNormal, hitNormalInObjectSpace
		);
		intersection.AddHit(hit);
	}

	return numSolutions > 0;
}

int CTorus::SolveIntersections(const CVector3d& direction, const CVector3d& start, double uArray[4]) const
{
	double ox = start.x;
	double oy = start.y;
	double oz = start.z;

	double dx = direction.x;
	double dy = direction.y;
	double dz = direction.z;

	double sumDSquerd = dx * dx + dy * dy + dz * dz;
	double e = ox * ox + oy * oy + oz * oz -
		m_radius * m_radius - m_tubeRadius * m_tubeRadius;
	double f = ox * dx + oy * dy + oz * dz;
	double fourRadiusSquared = 4.0 * m_radius * m_radius;

	complex croots[4];
	const int numComplexRoots = SolveQuarticEquation(
		complex(sumDSquerd * sumDSquerd), 
		complex(4.0 * sumDSquerd * f),
		complex(2.0 * sumDSquerd * e + 4.0 * f * f + fourRadiusSquared * dy * dy),
		complex(4.0 * f * e + 2.0 * fourRadiusSquared * oy * dy),
		complex(e * e - fourRadiusSquared * (m_tubeRadius * m_tubeRadius - oy * oy)),
		croots
	);
	const int numRealRoots = FilterRealNumbers(numComplexRoots, croots, uArray);

	return numRealRoots;
}
