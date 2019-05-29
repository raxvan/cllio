
import srcbuild
import os

search = srcbuild.search
match = srcbuild.match
path = srcbuild.path

#os.environ['HOST_DIR']

srcbuild.generate({
	"name" : "cllio",
	"defines" : [
	],
	"kind" : 'exe',
	"incl" : [
		path("src/")
	],
	"src" : [] + 
		search("src",[".cpp",".h"])+ 
		search("tests/src",[".cpp",".h"]),

	"src-ignore" : []
})

