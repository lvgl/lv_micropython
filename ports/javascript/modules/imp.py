import sys
import builtins
import types

# keep the builtin function accessible in this module and via imp.__import__
__import__ = __import__

# Deprecated since version 3.4: Use types.ModuleType instead.
# but micropython aims toward full 3.4

# Return a new empty module object called name. This object is not inserted in sys.modules.
def new_module(name):
    return types.ModuleType(name)


def importer(name, *argv, **kw):
    global __import__
    try:
        return __import__(name, *argv)
    except ImportError:
        pass

    filen = ':{0}.py'.format(name)
    # print("INFO: getting online local version of", filen, file=sys.stderr)
    # todo open the file via open() or raise importerror
    try:
        code = open(filen, 'r').read()
    except:
        remote = False
        for i, path_url in enumerate(sys.path):
            if path_url.startswith('http://') or path_url.startswith('https://'):
                filen = '{0}/{1}.py'.format(path_url, name)
                # print("INFO: try to get online remote version of", filen, file=sys.stderr)
                try:
                    code = open(filen, 'r').read()
                    remote = True
                    break
                except:
                    continue
        else:
            raise ImportError('module not found')

    # build a empty module
    mod = types.ModuleType(name)

    mod.__file__ = filen

    # compile module from cached file
    try:
        code = compile(code, filen, 'exec')
    except Exception as e:
        sys.print_exception(e)
        raise

    # micropython would insert module before executing the whole body
    # and left it that way even on runtime error.
    sys.modules[name] = mod

    # execute it in its own empty namespace.
    try:
        ns = vars(mod)
    except:
        # print("WARNING: this python implementation lacks vars()", file=sys.stderr)
        ns = mod.__dict__

    try:
        exec(code, ns, ns)
    except Exception as e:
        sys.print_exception(e)
        # do not follow weird micropython behaviour and clean up zombie module
        del sys.modules[name]
        raise

    return mod


# install hook
builtins.__import__ = importer
# print("__import__ is now", importer)
