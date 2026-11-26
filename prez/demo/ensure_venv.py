"""Import this first: if unicorn or capstone is missing, rerun the script with the project's re/.venv Python."""
import os
import sys

VENV_PY = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "re", ".venv", "bin", "python")

try:
    import capstone, unicorn  # noqa: F401
except ImportError:
    if os.path.exists(VENV_PY) and not os.environ.get("TESORO_VENV_RETRY"):
        os.environ["TESORO_VENV_RETRY"] = "1"
        os.execv(VENV_PY, [VENV_PY] + sys.argv)
    sys.exit("unicorn/capstone not found: run with re/.venv/bin/python (see prez/README.md)")
