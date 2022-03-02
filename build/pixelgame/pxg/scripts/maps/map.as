class map
{
    map()
    {
        print("created\n");
    }

    ~map()
    {
        print("destructed\n");
    }

    void Start()
    {
        print("Start\n");
    }

    void FixedUpdate(float fElapsedTime)
    {
        print("FixedUpdate\n");
    }

    void Update(float fElapsedTime)
    {
        print("Update\n");
    }

    void End()
    {
        print("End\n");
    }
}