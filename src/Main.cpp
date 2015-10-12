
#include <UserInterface/Window.hpp>
#include <UserInterface/Input.hpp>
#include <Graphics/Graphics.hpp>
#include <Platforms/PlatformBootstrapper.hpp>

#include <Libs/Concurrency/Concurrency.hpp>
#include <Libs/IoCC/IoCC.hpp>

#include <stdio.h>
#include <memory>
#include <math.h>


class VisualEffect
	:	public RH::Graphics::Updatable,
		public RH::Libs::EventDispatcher::DirectDispatcher
{
public:

	VisualEffect(RH::Graphics::UpdatedNode * parent)
		:	RH::Graphics::Updatable(parent)
	{}
};

class PulseEffect
	:	public VisualEffect
{
	float f {0.0f};
	int frame {0};

	std::weak_ptr<RH::Graphics::Node> node0;
	std::weak_ptr<RH::Graphics::Node> node1;

	bool finished {false};

public:

	class PulseFinishedEvent {};

	PulseEffect(
			RH::Graphics::UpdatedNode * parent,
			std::weak_ptr<RH::Graphics::Node> node0,
			std::weak_ptr<RH::Graphics::Node> node1)
		:	VisualEffect(parent),
			node0(node0),
			node1(node1)
	{
	}

	virtual void Update() override {

		if(!finished) {

			f+=0.02;
//			node0.lock()->SetScale( 1.0f + sin(f) * 0.5 );
//			node1.lock()->SetScale( 1.0f + sin(f) * 0.5 );

			parent->SetScale( 1.0f + sin(f) * 0.5 );

			if(++frame >= 180) {
				finished = true;
				this->Raise( PulseFinishedEvent() );
			}
		}
	}
};



class MyScene
	:	public RH::Graphics::UpdatedNode,
		public RH::Libs::IoCCBase
{
	std::shared_ptr<RH::Graphics::SpriteNode> sprite0;
	std::shared_ptr<RH::Graphics::SpriteNode> sprite1;

	std::shared_ptr<RH::Graphics::Abstract::Bitmap> bitmap;
	std::shared_ptr<RH::Graphics::Abstract::MotionVideo> motionVideo;

	std::shared_ptr<PulseEffect> pulseEffect;
	std::unique_ptr<RH::Libs::EventDispatcher::ISubscription> pulseSubscription;

public:
	MyScene()
		:	RH::Graphics::UpdatedNode(NULL)
	{
		bitmap =
			  std::make_shared<RH::Graphics::Abstract::Bitmap>("base.png");

//		motionVideo =
//			std::make_shared<RH::Graphics::Abstract::MotionVideo>(this, "mv.kib");

		const char * str = "mv.ogg";
		motionVideo =
			IoCCBase::container.New<RH::Graphics::Abstract::MotionVideo>(
				static_cast<RH::Graphics::UpdatedNode*>(this), str);

//		motionVideo =
//			std::make_shared<RH::Graphics::Abstract::MotionVideo>(this, "mv.ogg");

		sprite0 = MakeExported<RH::Graphics::SpriteNode>( "sprite0", this, motionVideo );
//		sprite0 = MakeExported<RH::Graphics::SpriteNode>( "sprite0", this, /*motionVideo*/ bitmap );
//		sprite1 = MakeExported<RH::Graphics::SpriteNode>( "sprite1", this, /*motionVideo*/ bitmap );

		SetProjection( glm::ortho(0.0f, 1920.0f * 1.0f, 1080.0f * 1.0f, 0.0f) );
//		sprite1->SetTranslation(glm::vec3(1920.0f, 1080.0f, 0.0f));

		/*
		pulseEffect = std::make_shared<PulseEffect>(this, sprite0, sprite1);

		pulseSubscription =
			pulseEffect->Subscribe<PulseEffect::PulseFinishedEvent>(
				[=](const PulseEffect::PulseFinishedEvent &ev){this->OnFinishedPulsing(ev);});
		*/

	}

	void OnFinishedPulsing(const PulseEffect::PulseFinishedEvent &event) {

		printf("finishedPulsing\n");
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

//		myScene.SetEnabled(false);

		while(!exitFlag) {

			window.DispatchInputEvents();

			renderer.Render(&myScene, &window);
		}
	}
};

class IOCCSetup : public RH::Libs::IoCCBase {

	void CreateVideoDecodeJobQueue(int threads) {

		// Create a job queue for motion videos.
		std::shared_ptr<RH::Libs::Concurrency::ConcurrentJobQueue> concurrentJobQueue =
			std::make_shared<RH::Libs::Concurrency::ConcurrentJobQueue>(threads);

		// Store a copy in the container.
		this->container.Store<RH::Libs::Concurrency::IConcurrentJobQueue>(
			"video job queue",
			concurrentJobQueue);


		// Register instantiator with container.
		this->container.RegisterInstantiator
			<std::shared_ptr<RH::Graphics::Abstract::MotionVideo>(RH::Graphics::UpdatedNode*, const char *)>(

				[&](RH::Graphics::UpdatedNode * node, const char * s)
				{
					return std::make_shared<RH::Graphics::Abstract::MotionVideo>(
							node, s,
							this->container.Retrieve<RH::Libs::Concurrency::IConcurrentJobQueue>("video job queue"));
				}

			);
	}

public:

	void Setup() {

		CreateVideoDecodeJobQueue(1);
	}
};

int main() {

	IOCCSetup().Setup();

	MyApplicaion myApp;

	myApp.Run();

	return 0;
}

