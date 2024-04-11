#include "Bullet/Bullet.h"
#include "Reticle/Reticle.h"

#include "../../../Object/Model/Model.h"

#include <list>

class Player {
public:
	~Player();

	void Initialize(const std::string& filename);

	void SetReticle();

	void Update();

	void Draw();

	void Move();

	void Shoot();

	void OnCollision();

	const std::list<Bullet*>& GetBullets() { return bullets; }

	std::unique_ptr<Model> model;

private:
	std::unique_ptr<Reticle> reticle;

	std::list<Bullet*> bullets;

	Camera* camera;
};