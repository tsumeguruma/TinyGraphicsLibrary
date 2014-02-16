/*
 * Light.h
 */

#ifndef TGL_CORE_LIGHT_H_
#define TGL_CORE_LIGHT_H_

#include <memory>
#include <string>
#include <Eigen/Core>

namespace tgl {

class Light;
typedef std::shared_ptr<Light> LightPtr;

class Light {
public:
	Light(int index);
	virtual ~Light();

	void setName(const std::string& name) { name_ = name; }
	const std::string& name() const { return name_; }

	int index() const { return index_; }

	void setAmbient(float ambient[4]);
	void setDiffuse(float diffuse[4]);
	void setSpecular(float specular[4]);

	void setPosition(const double position[3]);
	void setPosition(const Eigen::Vector3d& position);

	const Eigen::Vector3d& position() const { return p_; }

	void setEnable(bool enable);
	bool enable() const { return isEnable_; }

	virtual void initializeConfiguration();

	virtual void update();

protected:
	std::string name_;
	int index_;

	bool isEnable_;
	float position_[4];
	Eigen::Vector3d p_;
};

} /* namespace tgl */

#endif /* TGL_CORE_LIGHT_H_ */
