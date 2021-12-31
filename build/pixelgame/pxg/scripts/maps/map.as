class map
{
    map()
    { }

    void BeforeMapLoad()
    {
        print("BeforeMapLoad\n");
    }

    void AfterMapLoad()
    {
        print("AfterMapLoad\n");
    }

    void OnMapFixedUpdate()
    {
        print("OnMapFixedUpdate\n");
    }

    void OnMapEnd()
    {
        print("OnMapEnd\n");
    }
}