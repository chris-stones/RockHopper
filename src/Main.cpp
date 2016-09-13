
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
	:	public RH::Graphics::Updatable
{
	typedef RH::Libs::EventDispatcher::DirectDispatcher DirectDispatcher;
	typedef RH::Libs::EventDispatcher::ISubscription ISubscription;

	std::shared_ptr<DirectDispatcher> directDispatcher;

public:

	VisualEffect(RH::Graphics::UpdatedNode * parent)
		:	RH::Graphics::Updatable(parent)
	{
		directDispatcher = std::make_shared<DirectDispatcher>();
	}

	virtual ~VisualEffect() {
	}

	template<typename _EventType>
	std::unique_ptr<ISubscription> Subscribe(std::function<void(const _EventType &)> callback, std::function<bool(const _EventType &)> test) {

		return directDispatcher->Subscribe(callback, test);
	}

	template<typename _EventType>
	std::unique_ptr<ISubscription> Subscribe(std::function<void(const _EventType &)> callback) {

		return directDispatcher->Subscribe(callback);
	}

	template<typename _EventType>
	std::unique_ptr<ISubscription> Subscribe(std::function<void(const _EventType &)> callback, const _EventType &compareEvent) {

		return directDispatcher->Subscribe(callback, compareEvent);
	}

	template<typename _EventType>
	void Raise(const _EventType &event) {

		// NOTE: Event handler COULD try to delete the event dispatcher while
		//	other events are waiting to be processed.
		//  create a new shared_ptr on the stack.
		std::shared_ptr<DirectDispatcher> lockedInDirectDispatcher =
			this->directDispatcher;

		lockedInDirectDispatcher->Raise(event);
	}
};

class PlayMotionVideoEffect : public VisualEffect {

	typedef ::RH::Graphics::Abstract::MotionVideo MotionVideo;

	std::shared_ptr<MotionVideo> mv;
	bool finished{false};

public:

	struct FinishedEvent {
		std::shared_ptr<MotionVideo> mv;
		FinishedEvent(std::shared_ptr<MotionVideo> mv) : mv(mv) {}
	};

	PlayMotionVideoEffect(RH::Graphics::UpdatedNode * parent, std::shared_ptr<MotionVideo> mv)
		:	VisualEffect(parent),
			mv(mv)
	{
		mv->Play();
	}

	virtual ~PlayMotionVideoEffect() {

		printf("~PlayMotionVideoEffect() 0\n");
		mv->Reset();
		printf("~PlayMotionVideoEffect() 1\n");
	}

	virtual void Update() override {

		if(!finished && mv->IsFinished()) {
			finished = true;
			printf("BEGIN: %p->Raise(%p)\n", this, mv.get());
			this->Raise(FinishedEvent(mv));
			printf("END:   %p->Raise(%p)\n", this, mv.get());
		}
	}
};

/*
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

			f+=0.02f;
//			node0.lock()->SetScale( 1.0f + sin(f) * 0.5 );
//			node1.lock()->SetScale( 1.0f + sin(f) * 0.5 );

			parent->SetScale( 1.0f + sin(f) * 0.5f );

			if(++frame >= 180) {
				finished = true;
				this->Raise( PulseFinishedEvent() );
			}
		}
	}
};
*/


class MyScene
	:	public RH::Graphics::UpdatedNode,
		public RH::Libs::IoCCBase
{
	std::shared_ptr<RH::Graphics::SpriteNode> sprite0;
	std::shared_ptr<RH::Graphics::SpriteNode> sprite1;

	std::shared_ptr<RH::Graphics::Abstract::Bitmap> bitmap;
	std::shared_ptr<RH::Graphics::Abstract::MotionVideo> motionVideo;

//	std::shared_ptr<PulseEffect> pulseEffect;
//	std::unique_ptr<RH::Libs::EventDispatcher::ISubscription> pulseSubscription;

	std::unique_ptr<RH::Libs::EventDispatcher::ISubscription> playMotionVideoSubscription;
	std::shared_ptr<PlayMotionVideoEffect> playMotionVideoEffect;


public:
	MyScene()
		:	RH::Graphics::UpdatedNode(NULL)
	{
		bitmap =
			this->container.New<RH::Graphics::Abstract::Bitmap>("base.png");

		const char * str = "mv.ogg";
		motionVideo =
			IoCCBase::container.New<RH::Graphics::Abstract::MotionVideo>(
				static_cast<RH::Graphics::UpdatedNode*>(this), str);

		sprite0 = this->container.New
			<RH::Graphics::SpriteNode, RH::Graphics::Node*, std::shared_ptr<RH::Graphics::Abstract::IResource>>
				(this, motionVideo);

		SetProjection( glm::ortho(0.0f, 1920.0f * 1.0f, 1080.0f * 1.0f, 0.0f) );

	}

	virtual ~MyScene() {

		printf("~MyScene()\n");
	}

	void PlayVideo() {

		printf("PLAY VIDEO\n");

		playMotionVideoEffect = std::shared_ptr<PlayMotionVideoEffect>(
			new PlayMotionVideoEffect(this, motionVideo));

		playMotionVideoSubscription = playMotionVideoEffect->Subscribe<PlayMotionVideoEffect::FinishedEvent>(
			[&](const PlayMotionVideoEffect::FinishedEvent &ev) { this->OnVideoFinishedPlaying(); } );
	}

	void OnVideoFinishedPlaying() {

		printf("video finished\n");

		PlayVideo();
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

		myScene.PlayVideo();

		while(!exitFlag) {

			window.DispatchInputEvents();

			renderer.Render(&myScene, &window);
		}
	}
};

class IOCCSetup : public RH::Libs::IoCCBase {

	void CreateMotionVideoInstantiator(int threads) {

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

		CreateMotionVideoInstantiator(1);
		CreateBitmapInstantiator();
		CreateSpriteInstantiator();
	}
};

int main() {

	IOCCSetup().Setup();

	MyApplicaion myApp;

	myApp.Run();

	return 0;
}

