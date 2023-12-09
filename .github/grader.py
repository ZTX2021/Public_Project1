from pathlib import Path
repo_root = Path(__file__).parent.parent

def subtask_badge():
  import os
  repo_id = os.environ['REPO_ID']
  badge_md = f"[![](https://coverage.cpp.studio/{repo_id}/badge.svg)](https://coverage.cpp.studio/{repo_id}/)"
  with open(repo_root / 'README.md', 'r') as f:
    readme = f.read()
  if badge_md in readme:
    print('ok.')
    return True
  print('not found.')
  print("\tAdd this line to the beginning of your README.md:")
  print('\t' + badge_md)
  return False

def subtask_factory():
  with open(repo_root / 'README.md', 'r') as f:
    readme = f.read().lower()
  keywords = ['method', 'pattern', 'design']
  for k in keywords:
    if k in readme:
      print('ok.')
      return True
  print('invalid answer.')
  return False

def subtask_format():
  invalid = []
  from subprocess import run
  files = run(['git', 'ls-files'], capture_output=True, text=True, cwd=repo_root).stdout.split('\n')
  for file in files:
    file = repo_root / file
    if file.suffix != '.cpp' and file.suffix != '.h':
      continue
    formatted_output = run(['clang-format', '-style=file', file], capture_output=True, text=True).stdout
    with open(file, 'r') as f:
      original = f.read()
    if formatted_output != original:
      invalid.append(file)
  if len(invalid) == 0:
    print('ok.')
    return True
  print('invalid files:')
  for file in invalid:
    print('\t', file)
  return False

def subtask_std(name):
  exe_path = repo_root / f'build/bin/subtask_{name}'
  if not exe_path.exists():
    print(f'Subtask {name}: not found.')
    return False
  from subprocess import run
  run([exe_path.resolve().as_posix()], check=True)
  print(f'Subtask {name}: ok.')
  return True

def subtask_coverage():
  import os
  return float(os.environ['OVERALL_COVERAGE'])

def main():
  score = 0

  print('Subtask factory: ', end='')
  if subtask_factory():
    score += 10

  print('Subtask format: ', end='')
  if subtask_format():
    score += 10

  std_subtasks = {
    'cast': 15,
    'wrap': 15,
    'literal': 15,
    'csv': 15,
    'xpath': 20,
    'concat': 30,
    'binary': 20,
  }
  for name, subscore in std_subtasks.items():
    if subtask_std(name):
      score += subscore

  coverage = subtask_coverage()
  print('Subtask coverage:', coverage)
  if coverage >= 94:
    print('\tExtra score +10.')
    score += 10

  score = min(score, 90)

  if coverage < 92:
    print('\tBasic requirement not met.')
    print('\tPrevious score:', score)
    score *= 0.8
    print('\tNow:', score)

  print('Subtask badge: ', end='')
  if not subtask_badge():
    print('\tMandatory subtasks not done!')
    print('\tPrevious score: ', score)
    score = 0

  print('Final score: ', score)

if __name__ == '__main__':
  main()
