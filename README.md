ADAPTVSQL
=========

Original version of the Givaudan client server engine. These sources worked on OS/2
and Windows workstations creating a client side cache to AS/400 whose architecturebtw makes the entire machine accessible as a single relational store. Note: the cacheis not data, it's db access plan state.As a solution this fit in the Flavors division sample workflow process which processeda few hundred requests for samples a week. Before the solution was deployed theclient server system was about to be kicked off the AS/400 due to it's load on an already overloaded machine. This solution could neatly solve that problem becausethe CS system used a package called QELIB with a relatively simple 21 call SQL CLI
which this dll replaced.This will still work with any RDBMS that has still has static SQL as most of the big onesdo but would have to be adapted, targets IBM DB/2.
