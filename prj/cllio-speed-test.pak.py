
import os

def construct(ctx):
	
	ctx.prop("type","exe")

	ctx.fscan("src: ../tests/perftest/")

	ctx.dependency("cllio.pak.py")