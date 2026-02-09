# Trinity-POW Quarterly Objectives (2026 Q1)

Roadmap tracker: https://github.com/t81dev/t81-roadmap/issues/22
Repo Phase 4 issue: https://github.com/t81dev/trinity-pow/issues/1

## Objective 1: Anti-Abuse Check Execution

- Deliverable: run all checks from `docs/phase4-threat-model.md` with deterministic reporting.
- Acceptance checks:
  - artifact `evidence/phase4/YYYY-MM-DD/pow-eval.json` includes pass/fail for each anti-abuse check.
  - rejected malformed inputs are counted and categorized.

## Objective 2: Difficulty and Entropy Evidence

- Deliverable: collect evaluation data for at least two difficulty settings.
- Acceptance checks:
  - artifact includes `accepted_nonce_count`, `rejected_nonce_count`, and entropy summary.
  - run metadata includes runtime contract version and VM pin.

## Objective 3: Quarterly Reporting Discipline

- Deliverable: maintain evidence updates that keep roadmap freshness green.
- Acceptance checks:
  - evidence updates are linked in roadmap status cadence.
  - Phase 4 tracker issue reflects latest artifact date and unresolved risks.
