# Documentation Generation

These instructions will walk you through generating the Argtable project's
documentation on your local system using the same documentation sources
as we use to create the online documentation found at
https://www.argtable.org

## Documentation overview

The Argtable project content is written using the MyST (Markedly Structured
Text) markup language (`.md` file extension) with Sphinx extensions, and
processed using Sphinx to create a formatted stand-alone website. Developers can
view this content either in its raw form as `.md` markup files, or you can
generate the HTML content and view it with a web browser directly on your
workstation. This same `.md` content is also fed into the Argtable project's
public website documentation area (maybe with a different theme applied).

You can read details about [MyST](https://myst-parser.readthedocs.io), and
[Sphinx](https://www.sphinx-doc.org) from their respective websites.

The project's documentation contains the following items:

- MyST source files used to generate documentation found at the
  https://www.argtable.org website. All of the MyST sources can be found in the
  `/docs/source` directory.

- Doxygen-generated material used to create all API-specific documents
  also found at <https://www.argtable.org>

The MyST files are processed by the Sphinx documentation system, and make use of
the breathe extension for including the Doxygen-generated API material.
Additional tools are required to generate the documentation locally, as
described in the following sections.

## Installing the documentation processors

Our documentation processing has been tested to run with:

- Doxygen version 1.10.0
- Sphinx version 7.3.7
- Breathe version 4.35.0
- Furo Sphinx Theme version 2024.4.27
- MyST Parser version 3.0.0

In order to install the documentation tools, first create a Python virtual
environment. Then install additional tools that are only required to generate
the documentation, as described below.

### Doxygen

On Ubuntu Linux:

```shell
$ sudo apt install doxygen
```

On Fedora Linux:

```shell
$ sudo dnf install doxygen
```

On Arch Linux:

```shell
$ sudo pacman -S doxygen
```

On macOS:

```shell
$ brew install doxygen
```

On Windows in an Administrator ``cmd.exe`` prompt:

```shell
$ choco install doxygen.install
```

### Sphinx, Breathe, Furo, and MyST

```shell
$ python --version
Python 3.11.5

$ python -m venv .py311
$ .py311/Scripts/activate.bat

(.py311) $ pip install --upgrade sphinx
(.py311) $ pip install --upgrade furo
(.py311) $ pip install --upgrade breathe
(.py311) $ pip install --upgrade myst-parser
```

## Documentation presentation theme

Sphinx supports easy customization of the generated documentation appearance
through the use of themes. Replace the theme files and do another `make
html` and the output layout and style is changed. The `furo` theme
is installed as part of the step you took in the getting started guide.

## Running the documentation processors

The `/docs` directory in your cloned copy of the Argtable project git
repo has all the `.md` source files, extra tools, and Makefile for
generating a local copy of the Argtable project's technical documentation.
Assuming the local Argtable project copy is in a folder `argtable3` in your home
folder, here are the commands to generate the html content locally:

```shell
(.py311) $ cd argtable3/docs
(.py311) $ doxygen
(.py311) $ make html
```

:::{warning}
The documentation build system creates copies in the build directory of every
`.md` file used to generate the documentation, along with dependencies
referenced by those `.md` files.

This means that Sphinx warnings and errors refer to the **copies**, and **not
the version-controlled original files**. Be careful to make sure you don't
accidentally edit the copy of the file in an error message, as these changes
will not be saved.
:::

Depending on your development system, it will take up to one minutes to collect
and generate the HTML content.  When done, you can view the HTML output with
your browser started at `docs/build/html/index.html`.

If you add or remove a files or want to build the documentation from scratch,
just run `make clean` and then `make html` again.
