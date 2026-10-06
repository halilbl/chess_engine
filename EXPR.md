#Why i switched from class to namespaces in move_generator:
As i was writing code, it appeared to me that the MoveGenerator class was becoming full of static or inline functions which didn't even require an object(stateless class). Hence, i decided to switch to namespaces instead. I was riding a porsche but i was barely doing 80.So, i switched to a corolla. 

##What i've learned:

###Internal Linkage:
I've found out that anonymous namespace members inherently has internal linkage. Which means that these
members are only accesible in their own file/translation unit. But they can be seen in other files as 
copies as well(every file has its own isolated copy of that member). Linker is fully unaware of these members and they can not be exported. 

###Anonymous namespaces:
I knew what they were but i was not aware that they could replicate encapsulation quite well. Them providing internal linkage for their members and having no explicit name lets us store private members in an nonymous namespace. Every anonymous namespace is unique to their TU(translation unit)/.cpp file.


##Static keyword inside and outside a class:
Static keyword in a class totally means ownership, ownership shifts to class instead of objects. Outside a class, it assigns variables internal linkage.
