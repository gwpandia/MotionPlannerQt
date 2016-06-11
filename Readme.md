Motion Planning Using Numeric Field Algorithm

This is a project for entering Intelligent Media Lab @ Department of Computer Science, National Chengchi University. 
Advisor is Tsai-Yen Li. 

Warning:

If you are a lab member of IM-Lab and who is considering do some copy and paste, I suggest you stop and write your own.
You can refer or make yourself understand more of the algorithm instead of copy and paste. 
Not even saying that Li is aware of this open source project here.

Change Log:

2016/06/12
- Add control point weight feature, you can set the weights in the user interface.
- Fix size_t / int warnings, and layout issue.
- QtCreator config now can build without Intel TBB. See pro file.

Description:

The algorithm behind this planner is similar to A* algorithm using numeric field as heuristic function.
And using back-tracing to find the path of a plane (X, Y, Theta)
It can be speeded up by using Minkowski Sum to precompute those collided configurations.

Notice that the There are some bugs in Path Smoother when I ported from old Java source to C++.

Dependencies:

Qt 5 above (Linux, VC++, Mingw are all ok)

Intel Thread Building Block (TBB) (optional, Can be set in QtCreator)
