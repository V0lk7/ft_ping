#!/bin/bash
# Short installation - Conventional Commits
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "🔧 Hooks git installation..."

# Copy hooks
cp "$SCRIPT_DIR/commit-msg" .git/hooks/commit-msg
chmod +x .git/hooks/commit-msg

# Configure template
git config commit.template "$SCRIPT_DIR/commit-template"

echo "✅ Installation complete !"
