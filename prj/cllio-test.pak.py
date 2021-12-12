
import os


def construct(ctx):
	
	ctx.prop("type","exe")
	ctx.prop("define global: TESTING_BUILD")

	ctx.fscan("src: ../tests/basic/")

	ctx.dependency("../../dev-platform/dev-platform.pak.py")
	ctx.dependency("cllio.pak.py")
