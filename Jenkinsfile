pipeline {
    agent any

    parameters {
        string(name: 'REPO_NAME',
               defaultValue: 'lab4-cicd',
               description: 'Імя репозиторію')
        string(name: 'BRANCH_NAME',
               defaultValue: 'refs/heads/main',
               description: 'Гілка')
    }

    stages {

        stage('Checkout') {
            steps {
                script {
                    def branch = params.BRANCH_NAME
                        .replaceFirst(/^refs\/heads\//, '')
                    echo "Клонуємо репозиторій, гілка: ${branch}"
                    git branch: branch,
                        url: 'https://github.com/nierq/lab4-cicd.git'
                }
            }
        }

        stage('Build') {
            steps {
                script {
                    bat 'C:\\msys64\\mingw64\\bin\\gcc.exe -o main.exe main.c'
                    if (!fileExists('./main.exe')) {
                        error "main.exe не знайдено. Етап Build міг пройти невдало."
                    }
                }
            }
        }

        stage('Define Variant') {
            steps {
                script {
                    def output = bat(
                        script: 'echo dummy | main.exe',
                        returnStdout: true
                    ).trim()
                    def outputLines = output.split('\n')
                    def variant = outputLines[outputLines.size()-2].trim() as Integer

                    if (variant == 1) {
                        testCases = [
                            ['input': 'hello',
                             'expected': 'HeLlO'],
                            ['input': '123456',
                             'expected': '123456'],
                            ['input': 'Hello world 123',
                             'expected': 'HeLlO WoRlD 123'],
                            ['input': '*/1-==1',
                             'expected': '*/1-==1']
                        ]
                    } else {
                        error "Непідтримуваний варіант: ${variant}"
                    }
                    env.VARIANT = variant.toString()
                }
            }
        }

        stage('Form report') {
            steps {
                script {
                    def commitSha  = bat(script: 'git rev-parse HEAD',
                                        returnStdout: true).trim()
                    def author     = bat(script: 'git log -1 --pretty=%%an',
                                        returnStdout: true).trim()
                    def commitMsg  = bat(script: 'git log -1 --pretty=%%s',
                                        returnStdout: true).trim()
                    def commitDate = bat(script: 'git log -1 --pretty=%%cd',
                                        returnStdout: true).trim()

                    reportContent = new StringBuilder()
                    reportContent.append("""<!DOCTYPE html>
<html><head><meta charset='UTF-8'>
<style>
  body { font-family: Arial, sans-serif; margin: 40px; }
  h1   { text-align: center; color: #333; }
  .info { background: #f5f5f5; padding: 15px;
          border-radius: 8px; margin-bottom: 20px; }
  table { width: 100%; border-collapse: collapse; }
  th { background: #4a90d9; color: white; padding: 10px; }
  td { padding: 8px; border: 1px solid #ddd; text-align: center; }
  .passed { color: green; font-weight: bold; }
  .failed { color: red; font-weight: bold; }
</style></head><body>
<h1>Test report</h1>
<div class='info'>
  <b>Repository:</b> ${params.REPO_NAME}<br>
  <b>Commit:</b> ${commitSha}<br>
  <b>Author:</b> ${author}<br>
  <b>Commit message:</b> ${commitMsg}<br>
  <b>Commit time:</b> ${commitDate}
</div>
<table>
<tr><th>Input</th><th>Expected</th>
    <th>Output</th><th>Test status</th></tr>""")
                }
            }
        }

        stage('Test') {
            steps {
                script {
                    testCases.each { testCase ->
                        def testOutput = bat(
                            script: "echo ${testCase.input} | main.exe",
                            returnStdout: true
                        ).trim().split('\n')

                        def result = testOutput[testOutput.size()-1].trim()
                        def passed = (result == testCase.expected)

                        reportContent.append("<tr>")
                        reportContent.append("<td>${testCase.input}</td>")
                        reportContent.append("<td>${testCase.expected}</td>")
                        reportContent.append("<td>${result}</td>")
                        reportContent.append("<td class='${passed ? "passed" : "failed"}'>${passed ? "Passed" : "Failed"}</td>")
                        reportContent.append("</tr>")

                        if (!passed) {
                            reportContent.append("</table></body></html>")
                            writeFile file: 'report.html',
                                      text: reportContent.toString()
                            error "Test failed: очікувалось '${testCase.expected}', отримано '${result}'"
                        }
                    }
                    reportContent.append("</table></body></html>")
                    writeFile file: 'report.html',
                              text: reportContent.toString()
                }
            }
        }
    }

    post {
        always {
            publishHTML([
                allowMissing:          true,
                alwaysLinkToLastBuild: true,
                keepAll:               true,
                reportDir:             '.',
                reportFiles:           'report.html',
                reportName:            'Test Report'
            ])
        }
    }
}