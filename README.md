# IntegrityContainer

This data structure is constant storage type.
Once initialized it records the elements inside it, with their order.

Inside it uses dynamically allocated array and Queue implementation.

It has 2 methods that allow the usage of entites inside in a sequential order.

      use()
      restore()

use() method returns the topmost element, and maintains the copy of the element that has gone out.
Once you restore the element by calling the restore() you're good to use the next element.
Also it is fatal for the program to call use() method more than once without equal no of restores.

restore() method puts the element that was in use at the last, thereby providing constant time sequential access to elements.

Note: use() amd restore() must be used in pairs and in the respective order.

Also the order can be restored by calling restore_order() and once created the container cannot be reassigned.
