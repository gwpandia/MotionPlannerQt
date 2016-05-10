Motion Planning Using Numeric Field Algorithm

This is a project for entering Intelligent Media Lab @ Department of Computer Science, National Chengchi University. 
Advisor is Tsai-Yen Li. 

Warning:
If you are a lab member of IM-Lab and who is considering do some copy and paste, I suggest you stop and write your own.
You can refer or make yourself understand more of the algorithm instead of copy and paste. 
Not even saying that Li is aware of this open source project here.

Description:

The algorithm behind this planner is similar to A* algorithm using numeric field as heuristic function.
And using back-tracing to find the path of a plane (X, Y, Theta)
It can be speeded up by using Minkowski Sum to precompute those collided configurations.

Dependencies:

Qt 5 above (Linux, VC++, Mingw are all ok)
Intel TBB
