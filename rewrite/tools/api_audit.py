"""API action coverage: server handler / verifyResponse case / seen in the native game logs.

python recomp/rewrite/tools/api_audit.py          # writes recomp/rewrite/tasks/api_audit.txt

Columns: id, path, H (a server handler: @register(id) in server/src/handlers.py), V (a case in the rewritten
APIManager::verifyResponse, src/net/APIManager_verify.cpp), ok / fail counts of `>>> api {"action": id,
"status": s}` lines in recomp/out/host/*.log (status 0 = ok).
"""
import datetime
import json
import re
from pathlib import Path

RECOMP = Path(__file__).resolve().parents[2]
ROOT = RECOMP.parent
actions = json.loads((ROOT / "Input" / "1.0.1" / "server_api.json").read_text(encoding="utf-8"))["actions"]
handlers = {int(m) for m in re.findall(r"@register\((\d+)\)", (ROOT / "server" / "src" / "handlers.py").read_text(encoding="utf-8"))}
cases = {int(m) for m in re.findall(r"case (\d+):", (RECOMP / "rewrite" / "src" / "net" / "APIManager_verify.cpp").read_text(encoding="utf-8"))}
ok, fail = {}, {}
for log in (RECOMP / "out" / "host").glob("*.log"):
    for m in re.finditer(r'>>> api \{"action": (\d+), "status": (-?\d+)', log.read_text(errors="replace")):
        a, s = int(m.group(1)), int(m.group(2))
        (ok if s == 0 else fail)[a] = (ok if s == 0 else fail).get(a, 0) + 1
lines = ["# API actions: server handler / verifyResponse case / seen in native game logs (recomp/out/host/*.log, \">>> api\")",
         f"# id  path  handler  verify-case  ok-count  fail-count   ({datetime.date.today()}; python recomp/rewrite/tools/api_audit.py)"]
for i, a in enumerate(actions):
    lines.append(f"{i:3d}  {a['path']:<36s} {'H' if i in handlers else '-'} {'V' if i in cases else '-'} "
                 f"{ok.get(i, 0):6d} {fail.get(i, 0):3d}")
unseen = [i for i in range(len(actions)) if i in handlers and not ok.get(i)]
lines.append(f"# handled but never seen succeeding ({len(unseen)}): " + " ".join(str(i) for i in unseen))
(RECOMP / "rewrite" / "tasks" / "api_audit.txt").write_text("\n".join(lines) + "\n", encoding="utf-8")
print(lines[-1])
