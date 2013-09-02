//Include irrlicht

class IrrEngineDevice
{
    IrrEngineDevice()
    {
        Init();
    }

    void Init()
    {
        // Irr Init
        
        RiftInterface.GetVRData();

        ObjectManager.Load();
    }

    matrix4 MatrixConvert(MyMatrix);
    vector3df MatrixConvert(MyVector)

    void run()
    {
        while(device->run())
        {
            RiftInterface.RiftFix();

            LeapInterface.Update();
            PhysicsManager.Update();

            //draw all
        }
    }
};
/////////////////////////////////////////////////

class Object
{
    void Setvisible(bool);
    void SetRotation(MyVector);
    void SetPosition(MyVector);
};

class Window;

class Leaf;

class Hand;

class Wind;

/////////////////////////////////////////////////

//Include irrlicht

class ObjectManager
{

public:
    
    void Load();

    Object GetObject;

private:

    //Objects
};

//////////////////////////////////////////////////

//include   OVR

struct VRData
{

};

class RiftInterface
{
    void GetVRData(VRData& a_data);

    void RiftFix(float &pitch, float &yaw, float &row);
};

////////////////////////////////////////////////////

class PhysicsManager
{
    void Update();
};


////////////////////////////////////////////////////

class LeapInterface
{
    void Update();
};

////////////////////////////////////////////////////
//include OVR IRR LEAP

struct MyMatrix
{

};

struct MyVector
{

};

class Util
{

};

///////////////////////////////////////////////////

//Include irrlicht

class Controler
{

};
///////////////////////////////////////////////////

//Include irrlicht

class Shader
{

};

//////////////////////////////////////////////////

//include IrrEngineDevice

int main()
{
    IrrEngineDevice e;

    e.run();

	return 0;
}
