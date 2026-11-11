#ifndef WORLD_HPP
# define WORLD_HPP

# include <set>
# include <vector>
# include <iostream>
# include <memory>
# include <map>

# include "Intersection.hpp"
# include "Camera.hpp"
# include "rt.hpp"
# include "GpuObjects.hpp"
# include "Light.hpp"
# include "AmbientLight.hpp"
# include "Texture.hpp"
# include "stb_image.h"


class AObject;
class Ray;

class World {
	private:
		std::set<Intersection>					intersects;
		std::vector<std::unique_ptr<AObject>>	objects;
		std::vector<Light>						lights;
		AmbientLight							amb;
		Camera									cam;
		std::map<std::string, int>				textureIndex;
		std::vector<Image>						textures;

		bool									CameraDefined = false;
		bool									AmbientDefined = false;
	public:
	// Constructors and Destructors
		World();

	// method
		void					SetUpMatrixWorld();
		AObject*				AddObject(ObjectId id);
		Light&					AddLight();
		void					printWorld() const;
		void					printIntersection();
		int						registerTexture(const std::string& path);

		void					setCamera(Camera &c);
		Camera&					getCamera();

		void setAmbientLight(AmbientLight &c);
		AmbientLight& getAmbientLight();

		static const char*		objectName(ObjectId id);
		GpuScene				buildGpuScene();
		float sceneScale() const;

		bool cameraDefined() {return CameraDefined;}
		bool ambientDefined() {return AmbientDefined;}

		bool markCamera()  {if (CameraDefined)  return false; CameraDefined  = true; return true;}
		
		bool markAmbient() {if (AmbientDefined) return false; AmbientDefined = true; return true;}

		// texture manipulation
	bool                       resolveTextures();
	const std::vector<Image>&  getTextures() const {return textures;}
};

#endif