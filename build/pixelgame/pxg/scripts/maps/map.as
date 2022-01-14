class map
{
    map()
    {
        print("created\n");
    }
    
    virtual void Start()
    {
        print("Start\n");
    }

    virtual void FixedUpdate(float fElapsedTime)
    {
        print("Update\n");
    }

    virtual void Update(float fElapsedTime)
    {
        print("FixedUpdate\n");
    }

    virtual void End()
    {
        print("End\n");
    }
}