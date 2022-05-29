#pragma once
#include "GeometryObjectWithInitialTransformImpl.h"

class CTorus :
	public CGeometryObjectWithInitialTransformImpl
{
public:
	CTorus(
		double radius = 1,
		double tubeRadius = 0.5,
		CVector3d const& center = CVector3d(),
		CMatrix4d const& transform = CMatrix4d());

	/*
	Пересечение луча со сферой
	*/
	virtual bool Hit(CRay const& ray, CIntersection& intersection)const;
private:
	int SolveIntersections(const CVector3d& direction, const CVector3d& vantage, double uArray[4]) const;

	double m_radius = 1.0;
	double m_tubeRadius = 0.5;
};