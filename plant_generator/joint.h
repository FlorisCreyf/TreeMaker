/* Copyright 2020 Floris Creyf
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef PG_JOINT_H
#define PG_JOINT_H

#include "math/math.h"

#ifdef PG_SERIALIZE
#include <boost/archive/text_oarchive.hpp>
#endif

namespace pg {
	class Joint {
		unsigned id;
		unsigned parentID;
		size_t pathIndex;
		Vec3 location;

		#ifdef PG_SERIALIZE
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive &ar, const unsigned int version)
		{
			(void)version;
			ar & id;
			ar & parentID;
			ar & pathIndex;
			ar & location;
		}
		#endif

	public:
		/** Each joint is attached to a control point in the path. */
		Joint(unsigned id=0, unsigned parentID=0, size_t pathIndex=0);
		/** Update the location if the path changes. */
		void updateLocation(Vec3 location);
		unsigned getID() const;
		unsigned getParentID() const;
		size_t getPathIndex() const;
		Vec3 getLocation() const;
	};
}

#endif
