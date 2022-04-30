class map
{
    olc::vi2d aa;

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
        aa.x += 1;
        print("FixedUpdate\n");
    }

    void Update(float fElapsedTime)
    {
        aa.y += 1;
        print("Update\n");
    }

    void End()
    {
        print(aa.str());
        print("\nEnd\n");
    }
}