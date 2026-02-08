# Phase 4 Threat Model and Evaluation Notes

Roadmap tracker: https://github.com/t81dev/t81-roadmap/issues/5  
Implementation issue: https://github.com/t81dev/trinity-pow/issues/1

## Scope

Document threat model assumptions, anti-abuse checks, and benchmark/evidence mapping for `trinity-pow`.

## Threat Model

### Assets

1. Nonce validity rules (difficulty + entropy constraints).
2. Deterministic verifier behavior.
3. Result metadata used in downstream benchmark comparisons.

### Adversaries

1. Spam submitter generating malformed nonce submissions.
2. Adversary attempting cheap low-entropy nonce bypass.
3. Replayer re-submitting old nonce sets under new benchmark claims.

### Trust Boundaries

1. CLI/API input boundary for nonce candidates and parameters.
2. Benchmark harness boundary where timing/throughput claims are recorded.
3. Artifact publication boundary where evidence is attached to release pins.

## Measurable Anti-Abuse Checks

| Check | Purpose | Pass Rule |
| --- | --- | --- |
| Entropy floor enforcement | block low-information nonces | `pass` when all accepted nonces satisfy configured entropy floor |
| Difficulty prefix verification | prevent acceptance of weak hashes | `pass` when each accepted nonce meets difficulty bytes rule |
| Replay marker logging | detect reused nonce claims across runs | `pass` when artifact includes run id + manifest pin and duplicate set detection |
| Input validation | prevent malformed parameter abuse | `pass` when invalid difficulty/depth inputs are rejected deterministically |

## Benchmark and Evidence Mapping

Phase 4 artifacts should map to `t81-benchmarks` publication flow:

1. Emit `pow-eval.json` with:
   - `accepted_nonce_count`
   - `rejected_nonce_count`
   - `avg_attempts_per_nonce`
   - `entropy_distribution_summary`
2. Include release pin metadata:
   - `manifest_contract_version`
   - `manifest_vm_pin`
3. Store under:
   - `evidence/phase4/YYYY-MM-DD/pow-eval.json`
