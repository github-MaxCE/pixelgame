class map
{
    map()
    {
        print("created\n");
    }
    
    void Start()
    {
        print("Start\n");
    }

    void FixedUpdate(float fElapsedTime)
    {
        print("Update\n");
    }

    void Update(float fElapsedTime)
    {
        print("FixedUpdate\n");
    }

    void End()
    {
        print("End\n");
    }
}