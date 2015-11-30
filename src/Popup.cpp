
#include <UserInterface/Window.hpp>
#include <UserInterface/Input.hpp>
#include <Graphics/Graphics.hpp>
#include <Platforms/PlatformBootstrapper.hpp>

#include <Libs/Concurrency/Concurrency.hpp>
#include <Libs/IoCC/IoCC.hpp>

#include <stdio.h>
#include <memory>
#include <math.h>

class PopupScene
	:	public RH::Graphics::Node,
		public RH::Libs::IoCCBase
{
	std::shared_ptr<RH::Graphics::SpriteNode> sprite;

	std::shared_ptr<RH::Graphics::Abstract::Bitmap> bitmap;

public:
	PopupScene(const char * popup)
		:	RH::Graphics::Node(NULL)
	{
		bitmap =
			this->container.New<RH::Graphics::Abstract::Bitmap>(popup);

		sprite = this->container.New
			<RH::Graphics::SpriteNode, RH::Graphics::Node*, std::shared_ptr<RH::Graphics::Abstract::IResource>>
				(this, bitmap);

		SetProjection( glm::ortho(0.0f, 1920.0f * 1.0f, 1080.0f * 1.0f, 0.0f) );


		// Centre the pop-up.
		SetTranslation(glm::vec3(
			(1920.0f - bitmap->GetWidth()) / 2.0f,
			(1080.0f - bitmap->GetHeight()) / 2.0f,
			0.0f));

		// Monitor is on its side. ( counter-clockwise ).
		SetRotation(glm::rotate(
				glm::mat4(1.0),
				-0.5f * 3.141592654f,
				glm::vec3(0.0f,0.0f,1.0f)));

	}

	virtual ~PopupScene() {

	}
};


class PopupApplicaion
{
	RH::UI::Window window;

	RH::Platform::PlatformBootstrapper platform;
	RH::Graphics::GraphicsAPI graphicsAPI;
	RH::Graphics::Renderer renderer;

	PopupScene scene;

public:

	PopupApplicaion(const char * popup)
		:	renderer(&graphicsAPI),
			scene(popup)
	{}

	void Run() {

		for(int i=0;i<9; i++)
			renderer.Render(&scene, &window);
	}
};

class IOCCSetup : public RH::Libs::IoCCBase {

	void CreateBitmapInstantiator() {

		this->container.RegisterInstantiator
			<std::shared_ptr<RH::Graphics::Abstract::Bitmap>(const char *)>(

				[&](const char * resource)
				{
					return std::make_shared<RH::Graphics::Abstract::Bitmap>(
						resource,
						this->container.Retrieve<RH::Graphics::TextureFactory>());
				}
			);
	}

	void CreateSpriteInstantiator() {

		this->container.RegisterInstantiator
			<std::shared_ptr<RH::Graphics::SpriteNode>(RH::Graphics::Node*, std::shared_ptr<RH::Graphics::Abstract::IResource>)>(

				[&](RH::Graphics::Node *node, std::shared_ptr<RH::Graphics::Abstract::IResource> resource)
				{
					return std::make_shared<RH::Graphics::SpriteNode>(node, resource);
				}
			);
	}

public:

	void Setup() {

		CreateBitmapInstantiator();
		CreateSpriteInstantiator();
	}
};

int main(int argc, char * argv[]) {

	const char * popup_fn = "popup.gfx";
	if(argc==2)
		popup_fn = argv[1];

	IOCCSetup().Setup();

	PopupApplicaion popupApplicaion(popup_fn);

	popupApplicaion.Run();

//	getchar();

	return 0;
}

