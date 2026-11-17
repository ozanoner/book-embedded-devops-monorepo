# Embedded DevOps in Practice, book examples

Code for the chapters of *Embedded DevOps in Practice* (Packt, B35769). Each directory is one
chapter and is self-contained: a dev container, an ESP-IDF project, its tests, and its own
GitHub workflows.

## Chapters

| Directory | Chapter | Application | Targets |
|---|---|---|---|
| `ch02/` | Getting DevOps-Ready | `hello` | esp32 |
| `ch03/` | Releasing Firmware Across Multiple Platforms | `blinky` | esp32, esp32c3 |
| `ch04/` | Automating Integration Testing and Release Cycles | `blinky` | esp32 |
| `ch05/` | Applying Espressif's DevOps Tooling in Practice | `button-app` | esp32, esp32c3 |
| `ch06/` | Designing Secure OTA Update Pipelines | `blinky` | esp32 |
| `ch06-servers/` | Chapter 6 infrastructure | SignServer, EJBCA and runner stack | none |

Chapter 1 has no code. Chapters 7 to 12 are added as they are completed.

## Requirements

- Docker, and VS Code with the Dev Containers extension
- ESP-IDF v6.0, supplied by each chapter's container image
- An ESP32-DevKitC and an ESP32-C3-DevKitM-1
- A Raspberry Pi 4 as the self-hosted runner for Chapter 2
- A Wokwi CLI token for the simulation steps in Chapters 2, 4 and 5

## Working in a chapter

Each chapter is a complete project tree. Copy the chapter into its own repository before you
open it in a container, so git commands work where the book expects them:

```
cp -r ch04 ~/work/my-chapter-04
cd ~/work/my-chapter-04 && git init
```

Chapters 2, 4, 5 and 6 pass a `.env` file to the container, so create an empty one in the
chapter folder first and fill in the values each chapter asks for:

```
touch .env
```

Chapter 3 does not read `.env`.

Then open that folder in VS Code and reopen it in the container, which mounts the folder you
opened. Build the application that the chapter describes:

```
idf.py -C blinky build
```

The exact build and test commands are in the chapter's own `.github/workflows/` files and in
that chapter of the book.

## Tags and releases

Tags carry the chapter prefix: `ch02-v0.1`, `ch04-v0.2.0`, `ch06-v0.1.2`. The prefix keeps the
chapters apart in one repository. A release holds the firmware binaries for that chapter.
Chapter 6 downloads a tagged release image over the air, which is the OTA example.

## Repository layout

The only workflow at the repository root is `vault-build.yml`, and it runs on manual dispatch.
The chapter workflows inside `ch02/` to `ch06/` are for use when a chapter is copied out into
its own repository.

## License

MIT. See `LICENSE`.
