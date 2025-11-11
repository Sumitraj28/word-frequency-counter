


export function countWords(text) {
  const words = text.toLowerCase().match(/\b\w+\b/g);
  const freq = {};
  if (!words) return freq;
  for (const word of words) {
    freq[word] = (freq[word] || 0) + 1;
  }
  return freq;
}


function renderTable(freq) {
  const freqTable = document.getElementById('freqTable');
  const entries = Object.entries(freq).sort((a, b) => b[1] - a[1]);
  if (entries.length === 0) {
    freqTable.innerHTML = '<div class="text-gray-500">No words found.</div>';
    return;
  }
  let html = '<table class="min-w-full text-left border border-gray-200 rounded"><thead><tr><th class="px-4 py-2 bg-purple-100">Word</th><th class="px-4 py-2 bg-purple-100">Count</th></tr></thead><tbody>';
  for (const [word, count] of entries) {
    html += `<tr class="hover:bg-purple-50"><td class="px-4 py-2">${word}</td><td class="px-4 py-2">${count}</td></tr>`;
  }
  html += '</tbody></table>';
  freqTable.innerHTML = html;
}


function setupForm() {
  const form = document.getElementById('wordForm');
  const textInput = document.getElementById('textInput');
  const fileInput = document.getElementById('fileInput');
  const errorMsg = document.getElementById('errorMsg');
  const resultDiv = document.getElementById('result');
  const clearBtn = document.getElementById('clearBtn');

  form.addEventListener('submit', async (e) => {
    e.preventDefault();
    errorMsg.classList.add('hidden');
    resultDiv.classList.add('hidden');
    let text = textInput.value.trim();
    if (!text && fileInput.files.length === 0) {
      errorMsg.textContent = 'Please enter text or upload a .txt file.';
      errorMsg.classList.remove('hidden');
      return;
    }
    if (!text && fileInput.files.length > 0) {
      const file = fileInput.files[0];
      if (file.type !== 'text/plain') {
        errorMsg.textContent = 'Only .txt files are supported.';
        errorMsg.classList.remove('hidden');
        return;
      }
      text = await file.text();
    }
    if (!text) {
      errorMsg.textContent = 'No text found in the file.';
      errorMsg.classList.remove('hidden');
      return;
    }
    const freq = countWords(text);
    renderTable(freq);
    resultDiv.classList.remove('hidden');
  });

  clearBtn.addEventListener('click', () => {
    form.reset();
    document.getElementById('freqTable').innerHTML = '';
    resultDiv.classList.add('hidden');
    errorMsg.classList.add('hidden');
    textInput.value = '';
    fileInput.value = '';
    textInput.focus();
  });
}


window.addEventListener('DOMContentLoaded', setupForm); 