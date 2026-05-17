#include "doctor.h"
#include <iostream>
#include <algorithm>
#include <cctype>

// Static counter starts at 200 to match the original hospital_management.cpp
int Doctor::s_counter = 200;

// ── Constructors ──────────────────────────────────────────────────────────────

Doctor::Doctor(const std::string& name, int age,
               const std::string& spec, const std::string& hospitalId,
               const std::string& id)
    : Person(name, age), spec(spec), avail(true), hospitalId(hospitalId)
{
    if (id.empty()) {
        this->id = "DOC-" + std::to_string(++s_counter);
    } else {
        this->id = id;
    }
}

Doctor::Doctor(const std::string& id, const std::string& name, int age,
               const std::string& spec, const std::string& hospitalId)
    : Person(name, age), id(id), spec(spec), avail(true), hospitalId(hospitalId)
{
    // ID is provided (e.g. loaded from database) — no auto-generation needed
}

// ── Accessors ─────────────────────────────────────────────────────────────────

std::string Doctor::getID()         const { return id; }
std::string Doctor::getSpec()       const { return spec; }
bool        Doctor::isFree()        const { return avail; }
void        Doctor::setFree(bool v)       { avail = v; }
std::string Doctor::getHospitalId() const { return hospitalId; }
void        Doctor::setHospitalId(const std::string& hid) { hospitalId = hid; }

// ── displayInfo ───────────────────────────────────────────────────────────────

void Doctor::displayInfo() const {
    std::cout << "  Doctor ID   : " << id         << "\n"
              << "  Name        : " << name        << "\n"
              << "  Age         : " << age         << "\n"
              << "  Specialization: " << spec      << "\n"
              << "  Hospital    : " << hospitalId  << "\n"
              << "  Status      : " << (avail ? "Available" : "Busy") << "\n";
}

// ── diagnose ──────────────────────────────────────────────────────────────────
// Returns a {diagnosis, medicines} pair driven by the patient's actual symptoms.
// Each symptom keyword maps to a specific clinical finding and treatment.

std::pair<std::string, std::string> Doctor::diagnose(const std::string& symptoms) const
{
    // Lowercase copy for case-insensitive matching
    std::string s = symptoms;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);

    // ── Cardiology ────────────────────────────────────────────────────────────
    if (s.find("chest pain") != std::string::npos || s.find("chest tightness") != std::string::npos)
        return { "Acute chest pain — possible angina or myocardial infarction. Urgent ECG required.",
                 "Aspirin 300mg (stat) | Nitroglycerin 0.5mg sublingual | Morphine 2-4mg IV if needed" };

    if (s.find("palpitation") != std::string::npos || s.find("irregular heartbeat") != std::string::npos)
        return { "Cardiac arrhythmia suspected. Holter monitor and ECG recommended.",
                 "Metoprolol 25mg | Flecainide 100mg | Electrolyte correction if needed" };

    if (s.find("shortness of breath") != std::string::npos || s.find("breathless") != std::string::npos)
        return { "Dyspnea detected — possible heart failure or pulmonary congestion. Echo recommended.",
                 "Furosemide 40mg | Enalapril 5mg | Spironolactone 25mg" };

    if (s.find("high blood pressure") != std::string::npos || s.find("hypertension") != std::string::npos)
        return { "Hypertension confirmed. Lifestyle modification and medication required.",
                 "Amlodipine 5mg | Losartan 50mg | Hydrochlorothiazide 12.5mg" };

    if (s.find("heart") != std::string::npos || s.find("cardiac") != std::string::npos)
        return { "Cardiac irregularity detected. ECG and stress test recommended.",
                 "Aspirin 75mg | Atorvastatin 20mg | Metoprolol 25mg" };

    // ── Neurology ─────────────────────────────────────────────────────────────
    if (s.find("migraine") != std::string::npos)
        return { "Classic migraine with aura pattern identified. Trigger avoidance advised.",
                 "Sumatriptan 50mg | Ibuprofen 400mg | Propranolol 40mg (prophylaxis)" };

    if (s.find("seizure") != std::string::npos || s.find("convulsion") != std::string::npos)
        return { "Seizure episode reported. EEG and MRI brain recommended urgently.",
                 "Levetiracetam 500mg | Valproate 200mg | Diazepam 5mg (rescue)" };

    if (s.find("dizziness") != std::string::npos || s.find("vertigo") != std::string::npos)
        return { "Vestibular neuritis / central vertigo suspected. Balance assessment needed.",
                 "Betahistine 16mg | Cinnarizine 25mg | Prochlorperazine 5mg" };

    if (s.find("numbness") != std::string::npos || s.find("tingling") != std::string::npos)
        return { "Peripheral neuropathy or nerve compression suspected. Nerve conduction study advised.",
                 "Pregabalin 75mg | Vitamin B12 1000mcg | Methylcobalamin 500mcg" };

    if (s.find("headache") != std::string::npos || s.find("head pain") != std::string::npos)
        return { "Tension-type headache identified. Stress management and hydration advised.",
                 "Paracetamol 500mg | Ibuprofen 400mg | Amitriptyline 10mg (if chronic)" };

    if (s.find("memory loss") != std::string::npos || s.find("confusion") != std::string::npos)
        return { "Cognitive impairment noted. Dementia screening and MRI brain recommended.",
                 "Donepezil 5mg | Memantine 10mg | Vitamin E 400IU" };

    if (s.find("brain") != std::string::npos || s.find("neuro") != std::string::npos)
        return { "Neurological assessment complete. Further imaging may be required.",
                 "Paracetamol 500mg | Ibuprofen 400mg | Sumatriptan 50mg" };

    // ── Orthopedics ───────────────────────────────────────────────────────────
    if (s.find("fracture") != std::string::npos || s.find("broken bone") != std::string::npos)
        return { "Bone fracture confirmed on X-ray. Immobilization and casting required.",
                 "Calcium 1000mg + Vit-D 800IU | Ibuprofen 400mg | Tramadol 50mg (pain)" };

    if (s.find("back pain") != std::string::npos || s.find("lower back") != std::string::npos)
        return { "Lumbar strain / disc herniation suspected. Physiotherapy and rest advised.",
                 "Diclofenac 50mg | Cyclobenzaprine 5mg | Calcium + Vit-D supplement" };

    if (s.find("knee pain") != std::string::npos || s.find("knee") != std::string::npos)
        return { "Knee osteoarthritis or ligament injury suspected. MRI knee recommended.",
                 "Naproxen 500mg | Glucosamine 1500mg | Intra-articular steroid if needed" };

    if (s.find("shoulder pain") != std::string::npos || s.find("shoulder") != std::string::npos)
        return { "Rotator cuff injury or frozen shoulder suspected. Physiotherapy required.",
                 "Ibuprofen 400mg | Diclofenac gel topical | Physiotherapy exercises" };

    if (s.find("joint") != std::string::npos || s.find("arthritis") != std::string::npos)
        return { "Inflammatory arthritis / osteoarthritis identified. Joint protection advised.",
                 "Methotrexate 7.5mg (weekly) | Hydroxychloroquine 200mg | Folic acid 5mg" };

    if (s.find("bone") != std::string::npos || s.find("sprain") != std::string::npos)
        return { "Musculoskeletal strain. RICE protocol and physiotherapy advised.",
                 "Diclofenac 50mg | Calcium + Vit-D | Muscle relaxant 4mg" };

    // ── Pediatrics ────────────────────────────────────────────────────────────
    if (s.find("high fever") != std::string::npos || s.find("fever") != std::string::npos)
        return { "Febrile illness in child. Viral origin likely. Monitor for complications.",
                 "Paracetamol syrup 15mg/kg | ORS sachets | Zinc 20mg for 10 days" };

    if (s.find("cough") != std::string::npos && s.find("child") != std::string::npos)
        return { "Pediatric upper respiratory infection. Supportive care recommended.",
                 "Amoxicillin syrup 40mg/kg/day | Salbutamol inhaler | Saline nasal drops" };

    if (s.find("diarrhea") != std::string::npos || s.find("loose stool") != std::string::npos)
        return { "Acute gastroenteritis. Rehydration is the priority.",
                 "ORS sachets | Zinc 20mg | Probiotics | Avoid antibiotics unless bacterial" };

    if (s.find("vomiting") != std::string::npos || s.find("nausea") != std::string::npos)
        return { "Gastritis or viral gastroenteritis. Hydration and bland diet advised.",
                 "Ondansetron 4mg | ORS sachets | Domperidone 10mg | Omeprazole 20mg" };

    if (s.find("rash") != std::string::npos && s.find("child") != std::string::npos)
        return { "Pediatric viral exanthem (possible chickenpox or roseola). Isolation advised.",
                 "Calamine lotion | Cetirizine syrup | Paracetamol syrup | Acyclovir if severe" };

    if (s.find("child") != std::string::npos || s.find("kid") != std::string::npos || s.find("infant") != std::string::npos)
        return { "Pediatric general assessment complete. Mild viral illness suspected.",
                 "Paracetamol syrup | ORS sachets | Zinc supplement" };

    // ── ENT ───────────────────────────────────────────────────────────────────
    if (s.find("sore throat") != std::string::npos || s.find("throat pain") != std::string::npos)
        return { "Acute pharyngitis / tonsillitis. Strep throat must be ruled out.",
                 "Amoxicillin 500mg | Benzydamine gargle | Paracetamol 500mg | Lozenges" };

    if (s.find("ear pain") != std::string::npos || s.find("earache") != std::string::npos)
        return { "Acute otitis media suspected. Antibiotic therapy recommended.",
                 "Amoxicillin-Clavulanate 625mg | Ciprofloxacin ear drops | Paracetamol 500mg" };

    if (s.find("blocked nose") != std::string::npos || s.find("nasal congestion") != std::string::npos || s.find("sinus") != std::string::npos)
        return { "Acute sinusitis / rhinitis. Nasal irrigation and decongestants advised.",
                 "Amoxicillin-Clavulanate 625mg | Xylometazoline nasal spray | Cetirizine 10mg" };

    if (s.find("hearing loss") != std::string::npos || s.find("deaf") != std::string::npos)
        return { "Conductive or sensorineural hearing loss suspected. Audiometry required.",
                 "Betahistine 16mg | Prednisolone 40mg (if sudden) | Ear wax removal if needed" };

    if (s.find("nose bleed") != std::string::npos || s.find("epistaxis") != std::string::npos)
        return { "Epistaxis (nosebleed). Anterior nasal packing may be required.",
                 "Tranexamic acid 500mg | Vitamin K | Nasal saline spray | Avoid NSAIDs" };

    if (s.find("ear") != std::string::npos || s.find("nose") != std::string::npos || s.find("throat") != std::string::npos)
        return { "Upper respiratory tract infection / sinusitis.",
                 "Amoxicillin-Clavulanate 625mg | Nasal saline spray | Antihistamine 10mg" };

    // ── Dermatology ───────────────────────────────────────────────────────────
    if (s.find("acne") != std::string::npos)
        return { "Acne vulgaris (moderate-severe). Topical and systemic therapy required.",
                 "Doxycycline 100mg | Benzoyl peroxide 5% gel | Tretinoin 0.025% cream" };

    if (s.find("eczema") != std::string::npos || s.find("atopic") != std::string::npos)
        return { "Atopic dermatitis (eczema) flare. Emollient therapy and steroid cream advised.",
                 "Hydrocortisone cream 1% | Tacrolimus 0.1% ointment | Cetirizine 10mg | Moisturiser" };

    if (s.find("psoriasis") != std::string::npos)
        return { "Psoriasis plaque identified. Topical therapy and UV phototherapy considered.",
                 "Betamethasone cream | Calcipotriol ointment | Methotrexate 10mg (if severe)" };

    if (s.find("fungal") != std::string::npos || s.find("ringworm") != std::string::npos || s.find("tinea") != std::string::npos)
        return { "Dermatophytosis (fungal skin infection) confirmed. Antifungal therapy required.",
                 "Clotrimazole cream 1% | Fluconazole 150mg (weekly) | Keep area dry" };

    if (s.find("urticaria") != std::string::npos || s.find("hives") != std::string::npos)
        return { "Urticaria (hives) — allergic reaction. Trigger identification recommended.",
                 "Cetirizine 10mg | Fexofenadine 180mg | Prednisolone 20mg (if severe)" };

    if (s.find("rash") != std::string::npos || s.find("itch") != std::string::npos || s.find("skin") != std::string::npos)
        return { "Allergic skin reaction / contact dermatitis. Avoid identified triggers.",
                 "Hydrocortisone cream 1% | Cetirizine 10mg | Moisturiser twice daily" };

    // ── General Medicine fallback — symptom-specific ──────────────────────────
    if (s.find("cough") != std::string::npos)
        return { "Acute bronchitis / upper respiratory infection. Supportive care advised.",
                 "Amoxicillin 500mg | Dextromethorphan syrup | Paracetamol 500mg | Steam inhalation" };

    if (s.find("cold") != std::string::npos || s.find("runny nose") != std::string::npos)
        return { "Common cold (viral rhinitis). Rest and hydration recommended.",
                 "Paracetamol 500mg | Cetirizine 10mg | Vitamin C 1000mg | Zinc lozenges" };

    if (s.find("fatigue") != std::string::npos || s.find("weakness") != std::string::npos || s.find("tired") != std::string::npos)
        return { "Generalized fatigue — possible anemia, thyroid disorder, or viral illness. Blood work advised.",
                 "Ferrous sulfate 200mg | Vitamin B12 1000mcg | Multivitamin | Adequate rest" };

    if (s.find("stomach") != std::string::npos || s.find("abdominal pain") != std::string::npos || s.find("gastric") != std::string::npos)
        return { "Gastritis or peptic ulcer disease suspected. Endoscopy if symptoms persist.",
                 "Omeprazole 20mg | Antacid suspension | Metronidazole 400mg | Bland diet" };

    if (s.find("diabetes") != std::string::npos || s.find("blood sugar") != std::string::npos)
        return { "Diabetes mellitus — glycemic control assessment required. HbA1c test advised.",
                 "Metformin 500mg | Glibenclamide 5mg | Insulin (if HbA1c > 9%) | Dietary control" };

    if (s.find("urinary") != std::string::npos || s.find("burning urine") != std::string::npos || s.find("uti") != std::string::npos)
        return { "Urinary tract infection (UTI) suspected. Urine culture recommended.",
                 "Ciprofloxacin 500mg | Nitrofurantoin 100mg | Increase fluid intake | Cranberry extract" };

    if (s.find("allergy") != std::string::npos || s.find("allergic") != std::string::npos)
        return { "Allergic reaction identified. Allergen avoidance and antihistamine therapy.",
                 "Cetirizine 10mg | Fexofenadine 180mg | Prednisolone 20mg (if severe)" };

    // Final fallback
    return { "General systemic review complete. Likely viral or bacterial infection. Follow-up in 5 days.",
             "Amoxicillin 500mg | Paracetamol 500mg | Vitamin C 1000mg | Adequate rest and hydration" };
}
