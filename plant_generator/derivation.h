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

#ifndef PG_DERIVATION_H
#define PG_DERIVATION_H

#include "spline.h"

#ifdef PG_SERIALIZE
#include <boost/archive/text_oarchive.hpp>
#endif

namespace pg {
	struct Derivation {
		enum Arrangement {
			Alternate,
			Opposite,
			Whorled
		};

		int depth = 1;
		float stemDensity = 0.0f;
		float leafDensity = 0.0f;
		Spline stemDensityCurve;
		Spline leafDensityCurve;
		float stemStart = 0.0f;
		float leafStart = 0.0f;
		float lengthFactor = 1.0f;
		float radiusThreshold = 0.0f;
		Arrangement arrangement = Arrangement::Alternate;
		unsigned seed = 0;

		#ifdef PG_SERIALIZE
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive &ar, const unsigned int version)
		{
			(void)version;
			ar & depth;
			ar & stemDensity;
			ar & leafDensity;
			ar & stemDensityCurve;
			ar & leafDensityCurve;
			ar & stemStart;
			ar & leafStart;
			ar & radiusThreshold;
			ar & lengthFactor;
			ar & arrangement;
			ar & seed;
		}
		#endif /* PG_SERIALIZE */
	};
}

#endif /* PG_DERIVATION_H */
