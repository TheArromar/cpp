import re
import pandas as pd

# === Config ===
INPUT_CSV  = "job_description.csv"
OUTPUT_CSV = "cleaned_job_description.csv"

# Set this to the column that contains the raw job description text.
# If you leave it as None, the script will auto-pick the first text-like column.
COLUMN_WITH_TEXT = "job_description"  # e.g., "job_description" or "description" or "text"


# --- Helpers ---

def first_sentence(text: str) -> str:
    """Take text before the first full stop (.)"""
    idx = text.find('.')
    return text if idx == -1 else text[:idx]

def remove_prefix_phrase(text: str) -> str:
    """
    JOB RULE 4: Remove the phrase 'with experience in' (case-insensitive)
    from the start of the string.
    """
    return re.sub(r'^\s*with experience in\s*', '', text, flags=re.IGNORECASE)

def split_skills(text: str) -> list:
    """
    Split into items by common separators. Keep it simple and conservative.
    """
    # Normalize separators to commas
    text = re.sub(r'[;•|/]+', ',', text)
    # Also handle ' and ' that often appears in lists
    text = re.sub(r'\s+\band\b\s+', ',', text, flags=re.IGNORECASE)
    parts = [p.strip(" \t\n\r\"'") for p in text.split(',')]
    return [p for p in parts if p]


def clean_resume_skills(raw: str) -> tuple[str, str]:
    """
    Modified to implement JOB_DESCRIPTION rules:
    2) role  = split by 'needed', take [0]
    3) skills_src = take [1] (if present)
    4) remove 'with experience in'
    5) split again by '.', take [0]
    Returns (Role, Cleaned_Skills)
    """
    if not isinstance(raw, str):
        return ("", "")

    # Step 2 & 3: split by "needed"
    parts = re.split(r'\bneeded\b', raw, maxsplit=1, flags=re.IGNORECASE)
    role = parts[0].strip()
    skills_src = parts[1].strip() if len(parts) > 1 else ""

    # Step 4: remove "with experience in"
    skills_src = remove_prefix_phrase(skills_src)

    # Step 5: take text before first '.'
    skills_sentence = first_sentence(skills_src)

    # Normalize to a comma-separated list (keep dedupe/order)
    items = split_skills(skills_sentence)
    seen = set()
    cleaned = []
    for it in items:
        key = it.strip()
        if key and key not in seen:
            seen.add(key)
            cleaned.append(key)

    return role, ", ".join(cleaned)


# --- Load & Auto-pick text column if needed ---

df = pd.read_csv(INPUT_CSV)

if COLUMN_WITH_TEXT is None:
    text_cols = df.select_dtypes(include=["object"]).columns.tolist()
    preferred = [c for c in text_cols if c.lower() in ("job_description", "description", "job", "text")]
    if preferred:
        COLUMN_WITH_TEXT = preferred[0]
    elif text_cols:
        COLUMN_WITH_TEXT = text_cols[0]
    else:
        raise ValueError("No text-like (object dtype) columns found. Set COLUMN_WITH_TEXT to your job description column.")

if COLUMN_WITH_TEXT not in df.columns:
    raise ValueError(f"Configured COLUMN_WITH_TEXT='{COLUMN_WITH_TEXT}' not found in CSV columns: {list(df.columns)}")


# --- Apply cleaning ---

# Step 1: place JobID (unique ID per row, sequential)
job_ids = [f"J{str(i).zfill(6)}" for i in range(1, len(df) + 1)]

roles = []
cleaned_skills = []
for raw in df[COLUMN_WITH_TEXT]:
    role, skills = clean_resume_skills(raw)
    roles.append(role)
    cleaned_skills.append(skills)

out = pd.DataFrame({
    "JobID": job_ids,
    "Role": roles,                 # Role is allowed to repeat
    "Cleaned_Skills": cleaned_skills
})

# Remove rows with empty skills
out = out[out["Cleaned_Skills"].str.strip() != ""]

# Save with only the requested headers (order preserved)
out.to_csv(OUTPUT_CSV, index=False)

print(f"\nDone. Wrote {len(out)} rows to {OUTPUT_CSV}")