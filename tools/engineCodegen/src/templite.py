import sys, os
import re

class Templite(object):
    
    autowrite = re.compile(r'(^[\'\"])|(^[a-zA-Z0-9_\[\]\'\"]+$)')
    delimiters = ('${', '}$')
    cache = {}

    def __init__(self, text=None, filename=None,
                    encoding='utf-8', delimiters=None, caching=False):
        """Loads a template from string or file."""
        if filename is not None:
            filename = os.path.abspath(filename)
            mtime = os.path.getmtime(filename)
            self.file = key = filename
        elif text is not None:
            self.file = mtime = None
            key = hash(text)
        else:
            raise ValueError('either text or filename required')
        # set attributes
        self.encoding = encoding
        self.caching = caching
        if delimiters:
            start, end = delimiters
            if len(start) != 2 or len(end) != 2:
                raise ValueError('each delimiter must be two characters long')
            self.delimiters = delimiters
        # check cache
        cache = self.cache
        if caching and key in cache and cache[key][0] == mtime:
            self._code = cache[key][1]
            return
        # read file
        if filename:
            with open(filename, encoding=self.encoding) as fh:
                text = fh.read()
        self._code = self._compile(text)
        if caching:
            cache[key] = (mtime, self._code)
    
    def _compile(self, source):
        offset = 0
        tokens = ['# -*- coding: %s -*-' % self.encoding]
        start, end = self.delimiters
        escaped = (re.escape(start), re.escape(end))
        regex = re.compile('%s(.*?)%s' % escaped, re.DOTALL)

        # Split the source code using the regex
        parts = regex.split(source)

        for i, part in enumerate(parts):
            if i % 2 == 0:  # Static text
                # Strip a single newline before the template tag, if present
                if part.endswith("\n"):
                    part = part[:-1]  # Remove only the last newline

                if not part:
                    continue

                part = part.replace('\\', '\\\\').replace('"', '\\"')
                part = '\t' * offset + 'write("""%s""")' % part
            else:  # Inside a template tag
                part = part.strip()  # Remove surrounding whitespace/newlines from the template expression
                if not part:
                    continue
                
                part_stripped = part.lstrip()
                if part_stripped.startswith(':'):
                    if not offset:
                        raise SyntaxError('no block statement to terminate: ${%s}$' % part)
                    offset -= 1
                    part = part_stripped[1:]
                    if not part.endswith(':'):
                        continue
                elif self.autowrite.match(part_stripped):
                    part = 'write(%s)' % part_stripped
                
                lines = part.splitlines()
                margin = min(len(l) - len(l.lstrip()) for l in lines if l.strip())
                part = '\n'.join('\t' * offset + l[margin:] for l in lines)
                if part.endswith(':'):
                    offset += 1
            tokens.append(part)

        if source.endswith("\n"):
            tokens.append('write("\\n")')

        if offset:
            raise SyntaxError('%i block statement(s) not terminated' % offset)

        return compile('\n'.join(tokens), self.file or '<string>', 'exec')

    def render(self, **namespace):
        """Renders the template according to the given namespace."""
        stack = []
        namespace['__file__'] = self.file
        # add write method
        def write(*args):
            for value in args:
                stack.append(str(value))
        namespace['write'] = write
        # add include method
        def include(file):
            if not os.path.isabs(file):
                if self.file:
                    base = os.path.dirname(self.file)
                else:
                    base = os.path.dirname(sys.argv[0])
                file = os.path.join(base, file)
            t = Templite(None, file, self.encoding,
                            self.delimiters, self.caching)
            stack.append(t.render(**namespace))
        namespace['include'] = include
        def to_class_name(text):
            s = text.replace("-", " ").replace("_", " ")
            s = s.split()
            if len(text) <= 1:
                return text
            if len(s) <= 1:
                return text[0].capitalize() + text[1:]
            out = ''.join(i.capitalize() for i in s[:])
            return out
        
        namespace['to_class_name'] = to_class_name
        # execute template code
        exec(self._code, namespace)
        return ''.join(stack)
    