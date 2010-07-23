/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 30/06/2010.
 */

#include "fieldkit/physics/space/BasicSpace.h"

using namespace fieldkit::physics;

BasicSpace::BasicSpace() {}

BasicSpace::BasicSpace(Vec3f offset, Vec3f dimension)
{
	init(offset, dimension);
}

BasicSpace::~BasicSpace() {}

void BasicSpace::init(Vec3f offset, Vec3f dimension)
{
	this->position = offset + dimension * 0.5f;
	this->extent = dimension * 0.5f;
	updateBounds();	
}

void BasicSpace::reserve(int count)
{
	spatials.reserve(count);
}

void BasicSpace::clear() 
{
	spatials.clear();
}

void BasicSpace::insert(SpatialPtr s) 
{
	spatials.push_back(s);
}

void BasicSpace::select(BoundingVolumePtr volume, SpatialListPtr result)
{
	result->clear();
	BOOST_FOREACH(SpatialPtr s, spatials) {
		if(volume->contains(s->getPosition())) {
			result->push_back(s);
		}
	}
}
