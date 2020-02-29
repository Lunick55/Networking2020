using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TestDriver : MonoBehaviour
{
    public int add = 10;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        add = a3_Network.foo(10);
        Debug.Log(add);
    }
}
