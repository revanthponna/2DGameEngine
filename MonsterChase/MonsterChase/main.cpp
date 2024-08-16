
#include "HelperFuncs.hpp"
#include <conio.h>
#include <limits>

#include "Mutex.h"
#include "ScopeLock.h"
#include "CollisionSystem.hpp"

using namespace std;

Moveable* gPlayer = nullptr;

static constexpr float FORCE { 0.0001f };

using json = nlohmann::json;

void KeyCallBack (unsigned int i_VKeyID, bool bWentDown)
{
#ifdef _DEBUG
	const size_t	lenBuffer = 65;
	char			Buffer[lenBuffer];

	sprintf_s(Buffer, lenBuffer, "VKey 0x%04x went %s\n", i_VKeyID, bWentDown ? "down" : "up");
	OutputDebugStringA(Buffer);
#endif // __DEBUG

	if (bWentDown) {

		if (i_VKeyID == 0x0044)
		{
			gPlayer->AddForce(Vector2D (FORCE, 0));
		}
		else if (i_VKeyID == 0x0053)
		{
			gPlayer->AddForce(Vector2D(0, -FORCE));
		}
		else if (i_VKeyID == 0x0057)
		{
			gPlayer->AddForce(Vector2D(0, FORCE));
		}
		else if (i_VKeyID == 0x0041)
		{
			gPlayer->AddForce(Vector2D(-FORCE, 0));
		}
	}

	if (!bWentDown) {

		if (i_VKeyID == 0x0044)
		{
			gPlayer->AddForce(Vector2D(-FORCE, 0));
		}
		else if (i_VKeyID == 0x0053)
		{
			gPlayer->AddForce(Vector2D(0, FORCE));
		}
		else if (i_VKeyID == 0x0057)
		{
			gPlayer->AddForce(Vector2D(0, -FORCE));
		}
		else if (i_VKeyID == 0x0041)
		{
			gPlayer->AddForce(Vector2D(FORCE, 0));
		}
	}
}

void ProcessFileContents(uint8_t* i_pFileContents, size_t i_sizeFileContents, std::function<void(uint8_t*, size_t)> i_Processor)
{
	if (i_pFileContents)
	{
		if (i_sizeFileContents && !Engine::JobSystem::ShutdownRequested())
			i_Processor(i_pFileContents, i_sizeFileContents);
	}

	delete[] i_pFileContents;

	std::cout << "ProcessFileContents finished processing file.\n";
}

class ProcessFile
{
public:
	ProcessFile(const char* i_pFilename, std::function<void(uint8_t*, size_t)> i_Processor, const Engine::HashedString i_QueueName = Engine::JobSystem::GetDefaultQueue(), Engine::JobSystem::JobStatus* i_pJobStatus = nullptr) :
		m_pFilename(i_pFilename),
		m_Processor(i_Processor),
		m_QueueName(i_QueueName),
		m_pJobStatus(i_pJobStatus)
	{
		assert(m_pFilename);
	}

	void operator() ()
	{
		if (m_pFilename)
		{
			size_t sizeFileContents = 0;
			uint8_t* pFileContents = (uint8_t *) LoadFile(m_pFilename, sizeFileContents);

			if (pFileContents && sizeFileContents)
			{
				if (!Engine::JobSystem::ShutdownRequested())
				{
					std::cout << "ProcessFile finished loading file.\n";

					// this works around C++11 issue with capturing member variable by value
					std::function<void(uint8_t*, size_t)> Processor = m_Processor;

					Engine::JobSystem::RunJob(
						m_QueueName,
						[pFileContents, sizeFileContents, Processor]()
						{
							ProcessFileContents(pFileContents, sizeFileContents, Processor);
						},
						m_pJobStatus,
							"ProcessFileContents"
							);
				}
				else
				{
					delete[] pFileContents;
				}
			}
		}
	}

private:
	const char* m_pFilename;
	std::function<void(uint8_t*, size_t)>  m_Processor;
	Engine::HashedString m_QueueName;
	Engine::JobSystem::JobStatus* m_pJobStatus;
};



// Helper function to create new GameObjects from a list of names in a text file
void CreatePlayerObject(uint8_t* i_pFileContents, size_t i_sizeFileContents)
{
	assert(i_pFileContents && i_sizeFileContents);

	i_pFileContents[i_sizeFileContents - 1] = 0;

	json PlayerJSON = json::parse(i_pFileContents);

	if (PlayerJSON.contains("Player")) {

		json position = PlayerJSON["Player"]["Vector"];

		Vector2D pos;
		float mass;

		if (position.is_array()) {

			pos.SetXCoord(position[0]);
			pos.SetYCoord(position[1]);
		}

		SmartPtr<GameObject> go = GameObject::CreateNewGameObject(pos, pos);


		if (PlayerJSON["Player"].contains("Physics")) {

			mass = PlayerJSON["Player"]["Physics"]["Mass"];

			gPlayer = Physics::AddPhysicsObject(go, mass);
		}

		if (PlayerJSON["Player"].contains("Renderer")) {

		    std::string path = PlayerJSON["Player"]["Renderer"]["Sprite"];
			Renderer::AddRenderable(go, path.c_str());
		}
	}
}

void CreateGameObjects()
{
	using namespace std::placeholders;

	{
		const char* CustomQueueName = "GameObjectLoader";

		Engine::HashedString QueueName = Engine::JobSystem::CreateQueue(CustomQueueName, 1);

		Engine::JobSystem::RunJob(QueueName, std::bind(ProcessFile("Player.json", std::bind(CreatePlayerObject, _1, _2))));
		
	}
}

int WINAPI wWinMain(HINSTANCE i_hInstance, HINSTANCE /*i_hPrevInstance*/, LPWSTR /*i_lpCmdLine*/, int i_nCmdShow)
{

	// to check memory leaks after the program exits.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// IMPORTANT: first we need to initialize GLib
	bool bSuccess = GLib::Initialize(i_hInstance, i_nCmdShow, "GLibTest", (WORD) -1, 800, 600, true);

	Engine::JobSystem::Init();

	CreateGameObjects(); // create game objects

	Vector4D::Test();
	Matrix::Test();
	CollisionSystem::InitTestObjects ();

	if (bSuccess)
	{
		// IMPORTANT (if we want keypress info from GLib): Set a callback for notification of key presses
		GLib::SetKeyStateChangeCallback(KeyCallBack);

		bool bQuit = false;

		float dt;

		do
		{
			dt = Timer::CalcLastFrameTime_ms();

			// IMPORTANT: We need to let GLib do it's thing. 
			GLib::Service(bQuit);

			if (!bQuit)
			{			
				// Update physics
				Physics::Update(dt);

				// check for collisions and print debug statements
				CollisionSystem::FindCollision(dt);

				//Update Rendering
				Renderer::Update();
			}
		} while (bQuit == false);			

		Physics::ClearAll();
		Renderer::ClearAll();
		GameObject::ClearAll();
		CollisionSystem::ClearAll();

		// IMPORTANT:  Tell GLib to shutdown, releasing resources.
		GLib::Shutdown();

		Engine::JobSystem::RequestShutdown();

	}
}
