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


#How ripple carry algorithm works when computing subsets:

subset = 01100100 (8 bit)
value = 00000001
mask = 01100100

subset -= value & mask = 01100100 - 00000001 = 01100011 & 01100100 = 01100000 it 1 
subset -= value & mask = 01100000 - 00000001 = 01011111 & 01100100 = 01000010 it 2
.
.
.
.
.
.

##Why & operator:

To understand this, first remember that a subset can only have relevant bits as 1 (bits which are 1 and not 0). After substraction, 1's
will occur even on irrelevant bits as well. By using the & operator with mask, it is guaranteed that irrelevant bits will become 0.
What about relevant bits, opereator is being applied on them as well. Well it is applied but it does not change the bit to 0 or 1.
Because in the mask, relevant bits are always 1 and if the relevant bit is 0 in the subset then it is 0, if it is 1 then it is 1. Hence, no changes were done on relevant bits. This can basically be expressed and explained as a boolean function that is a dictator booleanfunction.



