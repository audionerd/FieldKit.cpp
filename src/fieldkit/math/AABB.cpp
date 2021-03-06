/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 24/05/2010.
 */

#include "fieldkit/math/AABB.h"
#include "fieldkit/math/SphereBound.h"

using namespace fieldkit;

// initialise to normalised box within [0,1] range
AABB::AABB() : BoundingVolume() 
{
	type = BoundingVolume::BOUNDING_BOX;
	position = Vec3f(0.5, 0.5, 0.5);
	setDimension(1, 1, 1);
}

AABB::AABB(float w, float h, float d) : BoundingVolume() 
{
	type = BoundingVolume::BOUNDING_BOX;
	position = Vec3f(w/2.0f, h/2.0f, d/2.0f);
	setDimension(w, h, d);
}

AABB::AABB(Vec3f const& min, Vec3f const& max) : BoundingVolume() 
{
	type = BoundingVolume::BOUNDING_BOX;
	set(min, max);
}

void AABB::set(Vec3f const& min, Vec3f const& max)
{
	this->min = min;
	this->max = max;
	extent = (max - min) * 0.5f;
	position = min + extent;	
}

void AABB::set(AABB const& box) 
{
	this->position = box.position;
	this->min = box.min;
	this->max = box.max;
	this->extent = box.extent;
}

void AABB::set(AABB* box) 
{
	this->position = box->position;
	this->min = box->min;
	this->max = box->max;
	this->extent = box->extent;
}
	
void AABB::setWidth(float value) 
{
	extent.x = value * 0.5f;
	updateBounds();
}
	
float AABB::getWidth() 
{
	return extent.x * 2.0f;
}

void AABB::setHeight(float value) 
{
	extent.y = value * 0.5f;
	updateBounds();
}
	
float AABB::getHeight() 
{
	return extent.y * 2.0f;
}

void AABB::setDepth(float value) 
{
	extent.z = value * 0.5f;
	updateBounds();
}
	
float AABB::getDepth() 
{
	return extent.z * 2.0f;
}
	
void AABB::setDimension(float w, float h, float d) 
{
	extent.x = w * 0.5f; 
	extent.y = h * 0.5f;
	extent.z = d * 0.5f;
	updateBounds();
}
	
Vec3f AABB::getDimension() 
{
	return extent * 2.0f;
}

// -- Bounding Volume ----------------------------------------------------------
bool AABB::contains(Vec3f const& p) 
{
	if(p.x < min.x || p.x > max.x) return false;
	if(p.y < min.y || p.y > max.y) return false;
	if(p.z < min.z || p.z > max.z) return false;
	return true;
}

// -- Helpers ------------------------------------------------------------------
void AABB::updateBounds() 
{
	min = position - extent;
	max = position + extent;
}

void AABB::expand( Vec3f const& amount )
{
	extent += amount;
	updateBounds();
}
