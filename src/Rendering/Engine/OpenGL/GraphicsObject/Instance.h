/**
 * Copyright 2024 Yuzhong Guo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "Object.h"
#include "Platform.h"
#include "GPUBuffer.h"
#include "Material.h"
#include "Topology/TriangleSet.h"
#include "Matrix/SquareMatrix.h"

namespace dyno
{
	class ShapeInstance : public GraphicsObject
	{
	public:
		ShapeInstance();
		~ShapeInstance() override;

		virtual void create();
		virtual void release();
		void update();
		void updateGL();

		DArray<Transform3f> transform;
		XBuffer<Transform3f> gltransform;
		int instanceCount = 0;

	private:
		bool mInitialized = false;
		
	};
};