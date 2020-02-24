using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;

public class a3_Network
{
    [DllImport("animal3D-Networking_x86")]
    static extern public int foo(int bar);


    [DllImport("animal3D-Networking_x86")]
    static extern public void sendVector(float x, float y);

    [DllImport("animal3D-Networking_x86")]
    static extern public void receiveVector(out float x, out float y);//In C = void receiveVector(float* x, float* y)
}
