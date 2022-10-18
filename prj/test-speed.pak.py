
def configure(cfg):
	cfg.link("cllio.pak.py")

def construct(ctx):

	ctx.config("type","exe")
	ctx.fscan("src: ../tests/perftest/")

	
