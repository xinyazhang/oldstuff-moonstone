Project Name:	Unnamed
Reversion	:	r15
Completion	:	low
Usablity	:	Perview Only

Context:
	虽然我原计划是准备把这个东西做到可用再发布的，但是现在看来我一个人的设计能力还是有限。
不得不提前发布，同时在此向各位寻求一下帮助（另一方面也给我点压力免得我又偷懒了）
	不过，在此之前不得不提一下这个东西是干什么的。作为一个松鼠党，我很早就认识到了用文件夹
进行分类的局限性，一个典型的情况就是大量从萌妹上收集下来的图。这使得我开始考虑编写一个类似
Danbooru的，基于Tag进行文件分类、归档、管理的工具。
	然而，由于多国语言和翻译等问题，普通的基于tag的工具是不够用的。
	比如说博丽灵梦就有N种写法：
		博丽灵梦
		博麗霊夢
		Reimu Hakurei
		Hakurei Reimu
	这势必要求程序将以上这些tag看成别名而已。这使得这个系统变得比danbooru的复杂了很多。
	现在系统内部的工作原理是这样的：
		博丽灵梦————————┐
		博麗霊夢————————┤
							    ├—概念
		Reimu Hakurei —————┤
		Hakurei Reimu —————┘
	这是一个以“概念”，而非tag为基础的系统。当然在界面上还是以“tag”和“别名”两种称呼表示。

	然后呢，“概念”（可以看成一组互为别名的tag）之间是可以有联系的：
		1. a kind of：例如“游戏”就可以关联到“东方永夜抄”上
		2. implication：“弹幕射击游戏”也是“游戏”，那么对“东方永夜抄”打上“弹幕射击游戏”的tag
			时，“东方永夜抄”这个概念会自动被打上“游戏”的Tag

	这样一来系统界面就非常复杂，以至于我现在根本无处下手了，下面我就简单描述一下我遇到的问题：

	1. Tag（概念）编辑的麻烦：
		之前已经提到“概念”这个东西是不会在界面中出现的（如同操作系统中文件的inode和hardlink一样）
		我们编辑和使用的都是tag，那么对tag的编辑必然涉及到下面几个问题：
		a. 编辑这个tag的名字，例如把博丽灵梦敲成部落联盟，那么就要修改这个tag
		b. 修改这个“概念”的名字——这里我们叫做主名（Primary name或master name）
		c. 将多个“概念”合并为一个
		d. 修改概念之间的关系

####	麻烦1：撤销和重做			####
	撤销和重作现在是必须的，关键问题是：

		这些编辑窗口是各自维护各自的状态，还是统一维护？

####	麻烦2：概念合并的撤销		####
	合并概念是最复杂的操作之一，按当前设计，概念合并后就很难分离了
	也许可以不支持已合并的概念的撤销操作

####	麻烦3：概念之间的关系		####
	这要求界面可以编辑一个概念，同时查看另一个概念，并且还有搜索功能
	这是不是太挤了点？求好的界面设计，设计方案可用QtDesigner展示给我看。

其他的功能先放一下。等我在kernel.dll中实现了再说。
