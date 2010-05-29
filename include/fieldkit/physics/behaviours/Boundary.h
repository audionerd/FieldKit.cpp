/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 29/05/2010.
 */

#pragma once

#include "fieldkit/FieldKit.h"
#include "fieldkit/physics/Particle.h"
#include "fieldkit/physics/Behaviour.h"

namespace fk { namespace physics {
	
	// Keeps a particle inside the given AABB volume by wrapping it around its edges without loosing velocity.
	class BoxWrap : public Behaviour, AABB {
	public:
		
		// whether the particles velocity should be cleared when it was wrapped
		bool preserveMomentum;
		
		BoxWrap() {
			preserveMomentum = false;
		}
		
		BoxWrap(AABB box) {
			set(box);
			preserveMomentum = false;
		}
		
		void apply(ParticlePtr p);
	};
	
} } // namespace fk::physics