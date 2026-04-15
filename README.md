# ft_ping

A re-implementation of the `ping` utility from [GNU inetutils](https://www.gnu.org/software/inetutils/), written in C.

---

## Table of Contents

- [About](#about)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Build Targets](#build-targets)
- [Project Structure](#project-structure)
- [Testing](#testing)
- [Contributing](#contributing)
- [License](#license)

---

## About

`ft_ping` is a from-scratch reimplementation of the classic `ping` network diagnostic tool, using the [inetutils ping](https://github.com/guillemj/inetutils) as a reference implementation. It sends ICMP ECHO_REQUEST packets to a target host and reports on round-trip time and packet loss.

> This project is built for educational purposes to understand raw sockets, ICMP protocol handling, and low-level network programming in C.

---

## Features

<!-- List the supported flags/behaviours once implemented TODO -->
- [ ] Send ICMP ECHO_REQUEST packets
- [ ] Report round-trip time (RTT) statistics
- [ ] Support standard options (e.g. `-c`, `-i`, `-t`, `-v` ...)
- [ ] IPv4 support
- [ ] Privilege handling (raw socket)

---

## Requirements

- **Compiler:** `clang`
- **OS:** Linux (or any POSIX-compliant system)
- **Privileges:** `ft_ping` requires raw socket access — run as `root`

---

## Installation

```sh
git clone https://github.com/V0lk7/ft_ping.git
cd ft_ping
make
```

This will produce the `ft_ping` binary in the project root.

---

## Usage

```sh
sudo ./ft_ping [OPTIONS]
```

### Examples

```sh
# Basic ping
sudo ./ft_ping google.com

# Send 5 packets
sudo ./ft_ping -c 5 google.com
```

### Options

| Option | Description |
|--------|-------------|
| ...    | *(to be documented)* |
<!-- TODO -->
---

## Build Targets

| Target        | Description                                              |
|---------------|----------------------------------------------------------|
| `make`        | Build in optimized release mode (`-O2`)                  |
| `make release`| Alias for `make all`                                     |
| `make debug`  | Build with debug symbols (`dwarf-5`, `ggdb3`)            |
| `make test`   | Clean rebuild and run the test suite with coverage       |
| `make clean`  | Remove build objects                                     |
| `make fclean` | Remove build objects and the `ft_ping` binary            |
| `make re`     | `fclean` + `all`                                         |
| `make help`   | Print available targets                                  |

---

## Project Structure

```
ft_ping/
├── app/
│   ├── include/        # Header files
│   ├── src/            # Source files
│   └── main.c          # Entry point
├── test/               # Test sources
├── lib/                # External libraries (if any)
├── githook/            # Contain files to ensure constitent commit format
├── Makefile
└── README.md
```

---

## Testing

Tests are built separately using a dedicated build directory and linked against an archive of the main sources. Test are using the greatest.h header from this git repository https://github.com/silentbicycle/greatest

```sh
make test
```

The test runner uses `greatest` in parallel mode (`greatest_parallel.sh`). Coverage is enabled automatically during test builds (`--coverage`).

---

## Contributing

Commits follow the [Conventional Commits](https://www.conventionalcommits.org/) specification:

```
<type>(scope)?: <description>

[optional body]

[optional footer(s)]
```

**Types:** `build` | `chore` | `ci` | `docs` | `feat` | `fix` | `perf` | `refactor` | `revert` | `style` | `test`

**Rules:**
- Header max 100 characters
- Type, scope and description in lowercase
- Description in present tense
- Breaking changes must include a `!` and a `BREAKING CHANGE:` footer

**Examples:**
```
feat(icmp): add echo request packet builder
fix: correct checksum calculation
fix!: remove deprecated raw socket fallback
```

You can enable the git hook to ensure the well formated commit by doing this
```sh
cd ft_ping
./githook/install.sh
```

---

## License

This project is licensed under the GNU General Public License v3.0 — see the [COPYING](COPYING) file for details.