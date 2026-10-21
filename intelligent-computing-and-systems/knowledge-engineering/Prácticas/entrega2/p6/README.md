# Rule-Based Recipe Recommendation

A collaborative CLIPS expert-system submission for filtering and presenting recipe recommendations.

## Objective and approach

Use a recipe fact base and inference rules to derive properties, filter candidate recipes, propose recipe types and present recommendations. The modules preserve their Spanish names because the main CLIPS program loads them by filename. [incertidumbre.clp](incertidumbre.clp) adds uncertainty-related rules.

## Implementation and evidence

- [p6.clp](p6.clp): main modules and rule orchestration.
- [BDrecetas_100.clp](BDrecetas_100.clp): recipe facts.
- [deducir_propiedades.clp](deducir_propiedades.clp), [filtrar.clp](filtrar.clp), [propuesta_tipo_receta.clp](propuesta_tipo_receta.clp), [presenta.clp](presenta.clp): reasoning and presentation.
- [Original report](memoria_p6.pdf): submitted documentation.

No recommendation-accuracy measurements or independent user evaluation logs are included. The artifact supports examination of rule-based reasoning, not a quantified recommendation-quality claim.

## Running and limitations

Requires CLIPS, running from this directory so relative module loads resolve. The supplied [run.clp](run.clp) requests `SE_clasificar_alimentos.clp`, which is **not included**. Its batch startup cannot be reproduced from this archive alone. Restore the original dependency before using `(batch* "run.clp")`; do not silently replace it with a guessed implementation.

## Academic context and authorship

**Team project:** source headers credit **Jaime Corzo Galdo and Mario Lindez Martinez**. Knowledge Engineering, Intelligent Computing and Systems track, Double Degree in Mathematics and Computer Science, University of Granada. Contributor names and recipe data are preserved.
