# GenericObjectPoolWithExample
This project is a generic object pool that can be used to store objects of any type. 
It is a header only library that can be included in any project.  
It is also a template class that can be used to store any type of object.

How to include it in your project:
1. Copy the GenericObjectPool.h file into your project.
2. Include the GenericObjectPool.h file in your project.
~~~cpp
#include "GenericObjectPool.h"
~~~
3. Create an instance of the GenericObjectPool class.
~~~
GenericObjectPool<YourClass> pool;
~~~