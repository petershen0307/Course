makefile
====

1. $@ is the name of the target. This is quite useful when the target is a pattern rather than fixed.

1. $^ is the name of the prerequisite that caused the rule to execute.

* % 是一個字串取代，代表dependence的檔名。比如說，test depend on main.o，main.o的檔名就是main，也就是%所代表的字串
* $? 代表需要重建的相依性項目
* $@ 目前的目標項目名稱
* $< 代表第一個相依性項目
* $* 代表第一個相依性項目，不過不含副檔名

[Makefile removes object files for no reason](https://stackoverflow.com/questions/15189704/makefile-removes-object-files-for-no-reason)

