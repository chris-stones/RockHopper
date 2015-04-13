
#include <UserInterface/Window.hpp>
#include <UserInterface/Input.hpp>
#include <Graphics/Graphics.hpp>
#include <Platforms/PlatformBootstrapper.hpp>
#include <stdio.h>
#include <memory>

class MyScene
	:	public RH::Graphics::UpdatedNode
{
	std::shared_ptr<RH::Graphics::SpriteNode> sprite0;

	std::shared_ptr<RH::Graphics::Abstract::Bitmap> bitmap;
	std::shared_ptr<RH::Graphics::Abstract::MotionVideo> motionVideo;

public:
	MyScene()
		:	RH::Graphics::UpdatedNode(NULL)
	{
		bitmap =
			//std::make_shared<RH::Graphics::Abstract::Bitmap>("sprite.tr");
			std::make_shared<RH::Graphics::Abstract::Bitmap>("GAME/LowerMonitor/Base/baseLower.tr");

//		motionVideo =
//			std::make_shared<RH::Graphics::Abstract::MotionVideo>(this, "mv.kib");

		sprite0 =
			MakeExported<RH::Graphics::SpriteNode>( "sprite0", this, /*motionVideo*/ bitmap );

		/*sprite0->*/SetProjection( glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f) );
	}
};


class MyApplicaion
	:	public RH::UI::InputSubscriberBase
{
	RH::UI::Window window;

	RH::Platform::PlatformBootstrapper platform;
	RH::Graphics::GraphicsAPI graphicsAPI;
	RH::Graphics::Renderer renderer;

	bool exitFlag {false};
	bool playFlag {true};

	std::unique_ptr<RH::Libs::EventDispatcher::ISubscription> keyPressedSubscription;

	MyScene myScene;

public:

	MyApplicaion()
		:	renderer(&graphicsAPI)
	{}

	void OnKeyPressed(const KeyPressedEvent &event) {

		if(event.GetKey() == RH::UI::Events::KEY_ESCAPE)
			exitFlag = true;

		if(event.GetKey() == RH::UI::Events::KEY_SPACE)
			playFlag = !playFlag;
	}

	void Run() {

		keyPressedSubscription =
			inputSubscriber.Subscribe<KeyPressedEvent>(
			[=](const KeyPressedEvent &ev) {this->OnKeyPressed(ev); });

		while(!exitFlag) {

			window.DispatchInputEvents();

			renderer.Render(&myScene, &window);

			if(playFlag)
				myScene.UpdateAll();
				//myScene.NextFrame();
		}
	}
};

int main() {

	MyApplicaion myApp;

	myApp.Run();

	return 0;
}

