
import os

def configure(cfg):
	cfg.link("cllio.pak.py")
	cfg.type("exe")

def construct(ctx):
	
	ctx.fscan("src: ../tests/perftest/")

	