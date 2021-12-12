
def construct(ctx):
	
	ctx.prop("type","lib")

	ctx.path("public include: ../incl")

	ctx.fscan("src: ../incl")
	ctx.fscan("src: ../src")

