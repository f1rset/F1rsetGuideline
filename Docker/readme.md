# Docker and dockerfiles

## ENV Dockerfiles

### 1. LazyVim and ZSH dockerfile

Link to File: [Dockerfile.nvim_zsh](Dockerfile.nvim_zsh)

- Build:

```bash
docker buildx build -t nvim_zsh_image . -f Dockerfile.nvim_zsh
```

- Run:

```bash
docker run -it -e "WAYLAND_DISPLAY=$WAYLAND_DISPLAY" -e "XDG_RUNTIME_DIR=$XDG_RUNTIME_DIR" -v "$XDG_RUNTIME_DIR:$XDG_RUNTIME_DIR" nvim_zsh_image
```

Display and XDG runtime dir is required for wl-clipboard to work inside container
